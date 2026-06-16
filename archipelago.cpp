#ifndef ARCHIPELAGO_CPP
#define ARCHIPELAGO_CPP
#include "archipelago.hpp"
#include <apclient.hpp>
#include <apuuid.hpp>
#include <string>
#include "apinterface.cpp"
#include "nametables.cpp"

using namespace ap;
using namespace hr;
using namespace nlohmann;

/*
UTILITY FUNCTIONS
*/

int ap::getNumberOfProgressedLands(progressCheck prog){ //prog needs to be at least "unlocked"
  int n=0;
  for(int i=0; i<eItem::ittypes; i++){
    eItem item = eItem(i);
    if(isTreasure(item) && ap::landChecksReceived[item]>=prog) n++;
  }
  return n;
}

bool ap::isTreasure(eItem item){
  return (iinf[item].itemclass==IC_TREASURE || item==itHolyGrail);
}

bool ap::isOrb(eItem item){
  return (iinf[item].itemclass==IC_ORB);
}

int ap::getVirtualTreasureCount(progressCheck prog){
  if(inv::on){
    switch (prog){
     case progressCheck::orbunlocked: return 25;
     case progressCheck::orbunlockedglobal: return 50;
     case progressCheck::completed: return 100;
     default: return 0;
    }
  } else {
    switch (prog){
     case progressCheck::orbunlocked: return 10;
     case progressCheck::orbunlockedglobal: return 25;
     case progressCheck::completed: return 50;
     default: return 0;
    }
  }
}

eItem ap::getItemByName(std::string name){
  for(int i=0; i<eItem::ittypes; i++){
    eItem item = eItem(i);
    if(iinf[item].name == name) return item;
  }
  return eItem::itNone;
}

/*
RANDOMIZER INITIALIZATION
*/

void ap::init::initRando(){
  deathLinkPending = false;
  landChecksReceived[itHyperstone] = progressCheck::unlocked;
  init::initItemByID();
  init::initOrbByID();
  return;
}

eLand ap::init::getFirstLand(){
  if(ap::settings::startLandID == -10){
    std::vector<eLand> validLands;
    for(int i=0;i<eItem::ittypes;i++){
      eItem item = eItem(i);
      if(isTreasure(item) && landChecksReceived[item]>=progressCheck::unlocked){
        if(std::find(invalidStartingLandNames.begin(), invalidStartingLandNames.end(), linf[landof(item)].name) == invalidStartingLandNames.end())
          validLands.push_back(landof(item));
      }
    }
    return validLands[hrand(validLands.size())];
  }
  if(ap::settings::startLandID < 0){
    switch (ap::settings::startLandID)
    {
    case -1:
      return laCrossroads;
    case -2:
      return laCrossroads2;
    case -3:
      return laCrossroads3;
    case -4:
      return laCrossroads4;
    case -5:
      return laCrossroads5;
    }
  }
  return landof(ap::itemByID[ap::settings::startLandID]);
}

/*
RANDOMIZER CHECK MANAGEMENT
*/

void ap::checks::hintLand(eLand land){
  LATE(
  if (client && client->get_state() >= APClient::State::SLOT_CONNECTED) {
    switch(landProgressChecksSent[linf[land].treasure]){
      case progressCheck::orbunlocked:
        if(settings::requiredTreasures >= 25){
          if(settings::extra_location_25){
            client -> LocationScouts({getLocationID(linf[land].treasure, progressCheck::orbunlockedglobal, (bool)(rand() % 2))}, 1);
          } else {
            client -> LocationScouts({getLocationID(linf[land].treasure, progressCheck::orbunlockedglobal, false)}, 1);
          }
        }
        break;

      case progressCheck::orbunlockedglobal:
        if(settings::requiredTreasures >= 50){
          if(settings::extra_location_50){
            client -> LocationScouts({getLocationID(linf[land].treasure, progressCheck::completed, (bool)(rand() % 2))}, 1);
          } else {
            client -> LocationScouts({getLocationID(linf[land].treasure, progressCheck::completed, false)}, 1);
          }
        }
        break;

      default:
        return;
    }
  });
}

void ap::checks::resetInventory(){
  for(int i=0; i<eItem::ittypes; i++){
    eItem item = eItem(i);
    if(isTreasure(item)){
      landChecksReceived[item] = ((item==itHyperstone) ? progressCheck::unlocked : progressCheck::locked);
    }
    if(isOrb(item)){
      orbsReceived[item] = 0;
      inv::extra_orbs[item] = 0;
    }
  }
  return;
}

void ap::checks::receiveCheck(APClient::NetworkItem netitem){
  if(netitem.index==0){
    ap_sync_queued = false;
    checks::resetInventory();
    checks::alreadyHandledChecks=-1;
  }
  if(netitem.index!=checks::alreadyHandledChecks+1) {
    checks::doFullSync();
    ap_sync_queued = true;
  } else {
    int item_id = netitem.item - HYPERROGUE_BASE_ID;
    if(item_id >= 0x1000){
      int orb_id = item_id -= 0x1000;
      inv::extra_orbs[orbByID[orb_id]]++;
      orbsReceived[orbByID[orb_id]]++;
    }
    else if(item_id >= 0){ // Crossroads (init) check and "Nothing" is excluded
      eItem item = itemByID[item_id];
      switch (landChecksReceived[item])
      {
      case progressCheck::locked: 
        landChecksReceived[item]=progressCheck::unlocked;
        break;
      case progressCheck::unlocked: 
        landChecksReceived[item]=progressCheck::orbunlocked;
        break;
      case progressCheck::orbunlocked: 
        landChecksReceived[item]=progressCheck::orbunlockedglobal;
        break;
      case progressCheck::orbunlockedglobal:
        landChecksReceived[item]=progressCheck::completed;
        break;
      case progressCheck::notingame:
        std::cout << "Received check for " << iinf[item].name << ", which is not in the game." << std::endl;
        break;
      default:
        break;
      }
    }
    checks::alreadyHandledChecks++;
  }
  return;
}

void ap::checks::collectCheck(eItem treasure, progressCheck progress){
  if (client && client->get_state() >= APClient::State::SLOT_CONNECTED) {
    if (treasure != itHyperstone && (progress == progressCheck::unlocked && !landUnlockCheckSent[treasure])){
      client -> LocationChecks({getLocationID(treasure, progress, false)});
    }
    if(treasure != itHyperstone && progress > landProgressChecksSent[treasure] && getVirtualTreasureCount(progress) <= settings::requiredTreasures){
      client -> LocationChecks({getLocationID(treasure, progress, false)});
    }
    if(treasure != itHyperstone && progress > landProgressChecksSent[treasure] && progress == progressCheck::orbunlocked && settings::extra_location_10){
      client -> LocationChecks({getLocationID(treasure, progress, true)});
    }
    if(treasure != itHyperstone && progress > landProgressChecksSent[treasure] && progress == progressCheck::orbunlockedglobal && settings::extra_location_25){
      client -> LocationChecks({getLocationID(treasure, progress, true)});
    }
    if(treasure != itHyperstone && progress > landProgressChecksSent[treasure] && progress == progressCheck::completed && settings::extra_location_50){
      client -> LocationChecks({getLocationID(treasure, progress, true)});
    }
  }
}

void ap::checks::updateChecks(){
  if(init::initialRestartDone){
    for(int i=0; i<eLand::landtypes; i++) {
      eLand l = eLand(i);
      eItem treasure = linf[l].treasure;

      if(ap::landProgressChecksSent[treasure] != progressCheck::notingame && !ap::landUnlockCheckSent[treasure] && landUnlockedLegacy(l)){
        checks::collectCheck(treasure, progressCheck::unlocked);
        ap::landUnlockCheckSent[treasure] = true;
      }
      if(landProgressChecksSent[treasure]==progressCheck::unlocked && items[treasure]>=(l==laCamelot ? 3 : getVirtualTreasureCount(progressCheck::orbunlocked))){
        checks::collectCheck(treasure, progressCheck::orbunlocked);
        ap::landProgressChecksSent[treasure] = progressCheck::orbunlocked;
      }
      if(landProgressChecksSent[treasure]==progressCheck::orbunlocked && items[treasure]>=(l==laCamelot ? 5 : getVirtualTreasureCount(progressCheck::orbunlockedglobal))){
        checks::collectCheck(treasure, progressCheck::orbunlockedglobal);
        ap::landProgressChecksSent[treasure] = progressCheck::orbunlockedglobal;
      }
      if(landProgressChecksSent[treasure]==progressCheck::orbunlockedglobal && items[treasure]>=(l==laCamelot ? 8 : getVirtualTreasureCount(progressCheck::completed))){
        checks::collectCheck(treasure, progressCheck::completed);
        ap::landProgressChecksSent[treasure] = progressCheck::completed;
      }
    }

    victoryAchieved = victoryAchieved || checks::checkWinCon();
    if(victoryAchieved && client && !victoryPackageSent) {
       victoryPackageSent = client->StatusUpdate(APClient::ClientStatus::GOAL);
    }
  }

  return;
}

bool ap::checks::checkWinCon(){
  switch (ap::settings::goal)
  {
  case ap::settings::goalCondition::hyperstones10:
    return items[itHyperstone]>=10;
  case ap::settings::goalCondition::hyperstones50:
    return items[itHyperstone]>=50;
  case ap::settings::goalCondition::orbofyendor:
    return items[itOrbYendor]>=1;
  default:
    printf("Goal not set\n");
    return false;
  }
}

void ap::checks::doFullSync(){
  if (client && client->get_state() >= APClient::State::SLOT_CONNECTED) {
    client->Sync();
    std::list<int64_t> locations = {};
    for(int i=0; i<eItem::ittypes; i++){
      eItem item = eItem(i);
      if(isTreasure(item)){
        if(landUnlockCheckSent[item]) locations.push_back(getLocationID(item, progressCheck::unlocked, false));
        if(landProgressChecksSent[item]>=progressCheck::orbunlocked) {
          locations.push_back(getLocationID(item, progressCheck::orbunlocked, false));
          locations.push_back(getLocationID(item, progressCheck::orbunlocked, true));
        }
        if(landProgressChecksSent[item]>=progressCheck::orbunlockedglobal) {
          locations.push_back(getLocationID(item, progressCheck::orbunlockedglobal, false));
          locations.push_back(getLocationID(item, progressCheck::orbunlockedglobal, true));
        }
        if(landProgressChecksSent[item]>=progressCheck::completed) {
          locations.push_back(getLocationID(item, progressCheck::completed, false));
          locations.push_back(getLocationID(item, progressCheck::completed, true));
        }
      }
    }
    client->LocationChecks(locations);
  }
}

/*
SETTINGS MANAGEMENT
*/
void ap::settings::readSettings(json settings){
  try{
    ap::settings::goal = static_cast<ap::settings::goalCondition>(settings["goal"]);
  } catch(std::exception err) {
    std::cout << "Goal not set" << std::endl;
    throw err;
  }
  ap::settings::deathLink = (bool) (int) settings["death_link"];
  ap::settings::startLandID = settings["starting_land"];
  ap::settings::requiredTreasures = settings["treasure_requirements"];
  ap::settings::hintOrb = (bool) (int) settings["hint_orb"];
  ap::settings::extra_location_10 = (bool) (int) settings["extra_location_10"];
  ap::settings::extra_location_25 = (bool) (int) settings["extra_location_25"];
  ap::settings::extra_location_50 = (bool) (int) settings["extra_location_50"];
}

void ap::sendDeathLink(std::string msg)
{
  if (!ap::settings::deathLink) return;
  deathtime = client->get_server_time();
  json data{
    {"time", deathtime},
    {"cause", msg}, // TODO: Player name
    {"source", client->get_slot()}
  };
  client->Bounce(data, {}, {}, {"DeathLink"});
  return;
}

// Might be interesting later:
/*
SAVE MANAGEMENT
*/

void ap::saves::writeApState(std::string fileName){
  std::ofstream statefile;
  statefile.open (fileName);
  statefile << "{" << std::endl;
  for(int i=0; i<eItem::ittypes; i++){
    eItem item = eItem(i);
    if(isTreasure(item)) {
      statefile << "\"" << iinf[item].name << "\": {" << std::endl;
      statefile << "  \"Received\":" <<(int) landChecksReceived[item] << "," << std::endl;
      statefile << "  \"Unlock Sent\":" << landUnlockCheckSent[item] << "," << std::endl;
      statefile << "  \"Progress Sent\":" << (int) landProgressChecksSent[item] << std::endl;
      statefile << "}," << std::endl;
    }
  }
  statefile << "\"Already handled checks\":" << checks::alreadyHandledChecks <<"\n}";
  statefile.close();
}

void ap::saves::readApState(std::string fileName) {
  std::ifstream i(fileName);
  if (i.is_open()) {
    json state;
    i >> state;
    i.close();
    for(int i=0; i<eItem::ittypes; i++){
      eItem item = eItem(i);
      if(isTreasure(item)){ //Technically not neccessary, but reduces json accesses
        json::iterator itementry = state.find(iinf[item].name);
        if(itementry!=state.end()){
          std::string landname = itementry.key();
          landChecksReceived[item]=(progressCheck) state[landname]["Received"];
          landUnlockCheckSent[item]=(bool) (int) state[landname]["Unlock Sent"];
          landProgressChecksSent[item]=(progressCheck) state[landname]["Progress Sent"];
        } else {
          landChecksReceived[item]=progressCheck::notingame;
        }
      }
    }
    checks::alreadyHandledChecks = state["Already handled checks"];
  } else {
    writeApState(fileName);
  }
  return;
}

#endif
