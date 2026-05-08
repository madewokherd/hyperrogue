#ifndef APINTERFACE_CPP
#define APINTERFACE_CPP
#include <apclient.hpp>
#include <apuuid.hpp>
#include <stdio.h>
#include <inttypes.h>
#include <unistd.h>
#include <algorithm>
#include <memory>
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#else
#define EM_BOOL bool
#define EM_TRUE true
#define EM_FALSE false
#if !defined WIN32 && !defined _WIN32
#include <poll.h>
#endif
#endif
#include <math.h>
#include <limits>

#define VERSION_TUPLE {0,6,7}


/*
 * This is the entry point for our cpp ap client.
 */


using nlohmann::json;

std::unique_ptr<APClient> client;
bool ap_socket_connect_sent = false;
bool ap_sync_queued = false;
bool ap_connect_sent = false; // TODO: move to APClient::State ?
bool is_https = false; // set to true if the context only supports wss://
bool is_wss = false; // set to true if the connection specifically asked for wss://
bool is_ws = false; // set to true if the connection specifically asked for ws://
unsigned connect_error_count = 0;
bool awaiting_password = false;
std::string slot = "Player";

#define OLD_DATAPACKAGE_CACHE "datapackage.json"
#define UUID_FILE "uuid" // TODO: place in %appdata%
#define CERT_STORE "cacert.pem"

#if __cplusplus < 201500L
decltype(APClient::DEFAULT_URI) constexpr APClient::DEFAULT_URI;  // c++14 needs a proper declaration
#endif

using namespace ap;

void disconnect_ap()
{
    client.reset();
}

void connect_slot(const std::string& password)
{
    if (client) {
        client->ConnectSlot(slot, password, 0b111, {"AP"}, VERSION_TUPLE);
        ap_connect_sent = true;
    } else {
        printf("Connection lost!\n");
    }
}

void connect_ap(std::string uri="", std::string newSlot="")
{
    ap_socket_connect_sent = true;
    client.reset();

    if (!newSlot.empty())
        slot = newSlot;

    is_ws = uri.rfind("ws://", 0) == 0;
    is_wss = uri.rfind("wss://", 0) == 0;

    std::string uuid = ap_get_uuid(UUID_FILE);

    printf("Connecting to AP...\n");
    client.reset(new APClient(uuid, "Hyperrogue", uri.empty() ? APClient::DEFAULT_URI : uri, CERT_STORE));

    // load DataPackage cache (deprecated)
    //try {
    //    client->set_data_package_from_file(OLD_DATAPACKAGE_CACHE);
    //} catch (std::exception) { /* ignore */ }

    // set state and callbacks
    ap_sync_queued = false;
    connect_error_count = 0;
    client->set_socket_connected_handler([](){
        printf("Socket connected\n");
    });
    client->set_socket_disconnected_handler([](){
        printf("Socket disconnected\n");
        ap_socket_connect_sent = false;
    });
    client->set_socket_error_handler([](const std::string& error) {
        connect_error_count++;
        if (!error.empty() && error != "Unknown")
            printf("%s\n", error.c_str());
        if(connect_error_count%5==0){       // Every 5 connect failures, reset entire connection attempt
            ap_socket_connect_sent = false;
        }
    });
    client->set_room_info_handler([](){
        connect_slot("");
    });
    client->set_slot_connected_handler([](const json& data){
        ap::settings::readSettings(data);

        ap::progressCheck landProgressChecksSentIncoming[eItem::ittypes]={};
        bool landUnlockCheckSentIncoming[eItem::ittypes]={};
        for(int i=0; i<eItem::ittypes; i++){
            landProgressChecksSentIncoming[i]=ap::progressCheck::unlocked;
        }
        std::set<int64_t> checked_locations = client->get_checked_locations();
        for(int loc_id: checked_locations){
            int land_id = (loc_id - HYPERROGUE_BASE_ID)%0X100;
            int progress_id = (loc_id - HYPERROGUE_BASE_ID) - land_id;
            eItem item = ap::itemByID[land_id];
            if(progress_id == 0){
                landUnlockCheckSentIncoming[item] = true;
            }
            else {
                switch (landProgressChecksSentIncoming[item])
                {
                case ap::progressCheck::unlocked: 
                    landProgressChecksSentIncoming[item]=ap::progressCheck::orbunlocked;
                    break;
                case ap::progressCheck::orbunlocked: 
                    landProgressChecksSentIncoming[item]=ap::progressCheck::orbunlockedglobal;
                    break;
                case ap::progressCheck::orbunlockedglobal:
                    landProgressChecksSentIncoming[item]=ap::progressCheck::completed;
                    break;
                case ap::progressCheck::notingame:
                    std::cout << "Server accepted check for " << iinf[item].name << ", which is not in the game." << std::endl;
                    break;
                default:
                    break;
                }
            }
        }
        bool haveToSync = false;
        for(int i=0; i<eItem::ittypes; i++){
            eItem item = eItem(i);
            if(ap::isTreasure(item)){
                if((!landUnlockCheckSentIncoming[item]) && ap::landUnlockCheckSent[item]){ //Happens if checks are collected while not online
                    haveToSync = true;
                }
                ap::landUnlockCheckSent[item] = ap::landUnlockCheckSent[item] || landUnlockCheckSentIncoming[item];
                if(landProgressChecksSentIncoming[item]<ap::landProgressChecksSent[item]){ //Happens if checks are collected while not online
                    haveToSync = true;
                } else {
                    ap::landProgressChecksSent[item] = landProgressChecksSentIncoming[item];
                }
            }
        }
        if(haveToSync){
            ap::checks::doFullSync();
            ap_sync_queued = true;
        }
        for(int i=0; i<eItem::ittypes; i++){
            eItem item = eItem(i);
            if(ap::isTreasure(item)){
                std::cout << iinf[item].name << ", Sent: " << (int) landProgressChecksSent[item] << ", Incoming Sent: " << (int) landProgressChecksSentIncoming[item] << std::endl;
            }
        }
        if(ap::settings::deathLink) client->ConnectUpdate(false, 0b111, true, {"AP", "DeathLink"});
        printf("Slot connected\n");
    });
    client->set_slot_disconnected_handler([](){
        printf("Slot disconnected\n");
        ap_connect_sent = false;
    });
    client->set_slot_refused_handler([](const std::list<std::string>& errors){
        ap_connect_sent = false;
        if (std::find(errors.begin(), errors.end(), "InvalidSlot") != errors.end()) {
            printf("Unknown or invalid slot: %s\n", slot.c_str());
        } else {
            printf("AP: Connection refused:");
            for (const auto& error: errors) printf(" %s", error.c_str());
            printf("\n");
        }
    });
    client->set_items_received_handler([](const std::list<APClient::NetworkItem>& items) {
        if (!client->is_data_package_valid()) {
            // NOTE: this should not happen since we ask for data package before connecting
            if (!ap_sync_queued) ap::checks::doFullSync();
            ap_sync_queued = true;
            return;
        }
        for (const auto& item: items) {
            ap::checks::receiveCheck(item);
            std::string itemname = client->get_item_name(item.item, client->get_player_game(client->get_player_number()));
            std::string sender = client->get_player_alias(item.player);
            std::string location = client->get_location_name(item.location, client->get_player_game(item.player));
            printf("  #%d: %s (%" PRId64 ") from %s - %s\n",
                   item.index, itemname.c_str(), item.item,
                   sender.c_str(), location.c_str());
        }
        // Once starting inventory has been received, (re-)start the Hyperrogue game.
        if(!ap::init::initialRestartDone){
            ap::init::initialRestartDone = true;
            hr::stop_game();
            hr::start_game();
        }
    });
    client->set_data_package_changed_handler([](const json& data) {
        printf("Data Package Changed\n");
    });
    client->set_print_handler([](const std::string& msg) {
        printf("%s\n", msg.c_str());
    });
    client->set_print_json_handler([](const std::list<APClient::TextNode>& msg) {
        printf("%s\n", client->render_json(msg, APClient::RenderFormat::ANSI).c_str());
        hr::addMessage(client->render_json(msg));
    });
    client->set_bounced_handler([](const json& cmd) {
        if (ap::settings::deathLink) {
        auto tagsIt = cmd.find("tags");
        auto dataIt = cmd.find("data");
        if (tagsIt != cmd.end() && tagsIt->is_array()
            && std::find(tagsIt->begin(), tagsIt->end(), "DeathLink") != tagsIt->end()) {
            if (dataIt != cmd.end() && dataIt->is_object()) {
            json data = *dataIt;
            if (data["time"].is_number() && std::abs(1-(data["time"].get<double>()/ap::deathtime))<=0.01) {
                ap::deathtime = -1;
            } else {
                ap::deathLinkPending = true;
                canmove = false;
                yasc_message = data.contains("cause") ? data["cause"] : "You were killed by an external force!";
                achievement_final(true);
                if(cmode & sm::NORMAL) showMissionScreen();
            }
            } else {
            printf("Bad deathlink packet!\n");
            }
        }
    }
    });
}

#endif
