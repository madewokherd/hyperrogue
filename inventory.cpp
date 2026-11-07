// Hyperbolic Rogue -- Orb Strategy Mode
// Copyright (C) 2011-2018 Zeno Rogue, see 'hyper.cpp' for details

/** \file inventory.cpp
 *  \brief Orb Strategy Mode
 */

#include "hyper.h"
namespace hr { 

/** \brief Implementation of the Orb Strategy Mode. 
 *
 * The most important functions called outside is hr::inv::show(). 
 */
EX namespace inv {

#if CAP_INV
  /** \brief is the Orb Strategy Mode active? */
  EX bool on;
  /** \brief the number of Orbs used up in each type */
  EX array<int, ittypes> usedup;
  /** \brief which Orb types have been unlocked */
  EX array<bool, ittypes> unlocked;
  /** \brief the number of points used so far */
  EX int usedpoints;
  /** \brief the number of points remaining */
  EX int points;
  /** \brief extra orbs can be added to OSM using -IX commandline option */
  EX array<int, ittypes> extra_orbs;

  /** \brief random seed used for hr::inv::invr */
  EX int rseed;
  /** \brief have we used any 'forbidden' orbs? */
  EX bool usedForbidden;
  
  /** \brief initialize the OSM data for a new game */
  EX void init() {
    rseed = hrandpos();
    usedForbidden = false;
    for(int i=0; i<ittypes; i++) usedup[i] = 0;
    }
  
  static constexpr int MIRRORED = 1000;
  static constexpr int TESTMIRRORED = 900;
  
  EX bool canbuy(eItem o) {
    return unlocked[o] && points >= usedup[o] + 1;
    }

  struct lateextraorb {
    eItem treasure;
    eItem orb;
    };
  
  vector<lateextraorb> lateextraorbs = {
    {itPower, itOrbFlash},
    {itPower, itOrbSpeed},
    {itPower, itOrbAether},
    {itPower, itOrbWinter},
  
    {itTrollEgg, itOrbFish},
    {itTrollEgg, itOrbStunning},
    {itTrollEgg, itOrbLuck},
    {itTrollEgg, itOrbLife},
    {itTrollEgg, itOrbDigging},
    {itTrollEgg, itOrbSpace},
  
    {itFulgurite, itOrbLightning},
    {itWindstone, itOrbSpeed},
    {itDragon, itOrbDragon},
    {itSlime, itOrbFlash},
    {itDodeca, itOrbShield},
    
    {itGreenGrass, itOrbHorns},
    {itGreenGrass, itOrbShield},
    {itGreenGrass, itOrbThorns}
    };

  /** \brief how many orbs can we get from Orb-of-Mirroring orb */
  int mirrorqty0(eItem orb) {
    if(shmup::on && isShmupLifeOrb(orb)) 
      return 3;
    if(orb == itOrbWater) return 10;
    if(orb == itOrbSummon) return 9;
    if(orb == itOrbEmpathy) return 9;
    if(orb == itOrbMatter) return 9;
    if(orb == itOrbIntensity) return 8;
    if(orb == itOrbLuck) return 8;
    if(orb == itOrbSpace) return 7;

    if(orb == itOrbWinter) return 6;
    if(orb == itOrbLife) return 6;
    if(orb == itOrbLove) return 6;
    if(orb == itOrbRecall) return 6;
    if(orb == itOrbDigging) return 6;
    if(orb == itOrbGravity) return 6;
    if(orb == itOrbImpact) return 6;
    
    if(orb == itOrbTime) return 5;
    if(orb == itOrbAir) return 5;
    if(orb == itOrbFish) return 5;
    if(orb == itOrbStunning) return 5;
    if(orb == itOrbUndeath) return 5;
    if(orb == itOrb37) return 5;
    if(orb == itOrbDomination) return 5;
    if(orb == itOrbBull) return 5;
    if(orb == itOrbHorns) return 5;

    if(orb == itOrbAether) return 4;
    if(orb == itOrbInvis) return 4;
    if(orb == itOrbFire) return 4;
    if(orb == itOrbDragon) return 4;
    if(orb == itOrbIllusion) return 4;
    if(orb == itOrbDiscord) return 4;
    if(orb == itOrbBeauty) return 4;

    if(orb == itOrbMirror) return 1;
    return 3;
    }
  
  int mirrorqty(eItem orb) {
    if(orb == itOrbMirror) return 1;
    return int(mirrorqty0(orb) * sqrt(1.000001+items[itPower]/20.));
    }
  
  /** \brief PRNG used for calculating how many Orbs you get for your collected treasure */
  std::mt19937 invr;
  
  /** \brief initialize hr::inv::invr */
  void sirand(int i) {
    invr.seed(i);
    }
  
  /** \brief get the next random value from hr::inv::invr */
  int irand(int i) {    
    return invr() % i;
    }
    
  EX eItem whichorbinfo;
  EX string orbinfoline, extra;
  
  string extraline(eItem it, string s) {
    return " "+XLAT1(iinf[it].name) + " ("+s+")";
    }
  
  void gainOrbs(eItem it, eItem o) {
    int qty = items[it];
    if (!unlocked[o] && qty >= ((it == itHolyGrail || it == itSavedPrincess) ? 1 : (ls::any_chaos() ? 5 : 10))) {
      unlocked[o] = true;
      }
    }

  int nextp2(int i) {
    int z = 1;
    while(z <= i) z <<= 1;
    return z;
    }

  vector<eItem> offensiveOrbs = {
    itOrbFlash, itOrbLightning, itOrbPsi, itOrbThorns,
    itOrbFreedom, itOrbSword, itOrbSword2,
    itOrbHorns, itOrbDragon, itOrbStunning
    };
  
  vector<eItem> elementalOrbs = {itOrbFire, itOrbWater, itOrbDigging, itOrbAir};

  vector<eItem> demonicOrbs = {itOrbFire, itOrbHorns, itOrbSummon};
  
  bool isIn(eItem o, vector<eItem>& l) {
    for(auto it: l) if(it == o) return true;
    return false;
    }

  void usedOrb(eItem orb)
    {
      usedup[orb]++;
      usedpoints += usedup[orb];
      points -= usedup[orb];
      items[itGreenStone] += abs(usedup[orb]);
    }
  
  /** \brief Compute how many orbs you get for your current treasure. This is called after every move, and should give consistent results */
  EX void compute() {
    extra = "";
    orbinfoline = "";

    int GOLD_PER_POINT = 25;
    int g = gold();
    if (g >= (usedpoints + points + 1) * GOLD_PER_POINT)
      points = g / 25 - usedpoints;

    sirand(rseed);
    
    gainOrbs(itShard, itOrbMirror);
    gainOrbs(itHyperstone, itOrbMirror);
    gainOrbs(itDiamond, itOrbFlash);
    gainOrbs(itGold, itOrbLife);
    gainOrbs(itSpice, itOrbShield);
    gainOrbs(itRuby, itOrbLightning);
    gainOrbs(itElixir, itOrbSpeed);
    gainOrbs(itBone, itGreenStone);
    gainOrbs(itHell, itOrbYendor);
    gainOrbs(itStatue, itOrbTeleport);
    gainOrbs(itFeather, itOrbSafety);
    gainOrbs(itSapphire, itOrbMorph);
    gainOrbs(itFernFlower, itOrbThorns);
    gainOrbs(itWine, itOrbAether);
    gainOrbs(itSilver, itOrbDigging);
    gainOrbs(itRoyalJelly, itOrbInvis);
    gainOrbs(itEmerald, itOrbPsi);
    gainOrbs(itPower, itOrbFire);
    gainOrbs(itHolyGrail, itOrbIllusion);
    gainOrbs(itGrimoire, itOrbDragon);
    gainOrbs(itPirate, itOrbTime);
    gainOrbs(itRedGem, itOrbSpace);
    gainOrbs(itBombEgg, itOrbFriend);
    gainOrbs(itCoast, itOrbEmpathy);
    gainOrbs(itWhirlpool, itOrbWater);
    gainOrbs(itPalace, itOrbDiscord);
    gainOrbs(itFjord, itOrbFish);
    gainOrbs(itSavedPrincess, itOrbLove);
    gainOrbs(itIvory, itOrbMatter);
    gainOrbs(itZebra, itOrbFrog);
    gainOrbs(itElemental, itOrbSummon);
    gainOrbs(itFulgurite, itOrbStunning);
    gainOrbs(itMutant, itOrbWoods);
    gainOrbs(itMutant2, itOrbFreedom);
    gainOrbs(itLotus, itOrbUndeath);
    gainOrbs(itWindstone, itOrbAir);
    gainOrbs(itRose, itOrbBeauty);
    gainOrbs(itCoral, itOrb37);
    gainOrbs(itBabyTortoise, itOrbShell);
    gainOrbs(itApple, itOrbEnergy);
    gainOrbs(itDragon, itOrbDomination);
    gainOrbs(itKraken, itOrbSword);
    gainOrbs(itBarrow, itOrbSword2);
    gainOrbs(itTrollEgg, itOrbStone);
    gainOrbs(itSlime, itOrbRecall);
    gainOrbs(itAmethyst, itOrbNature);
    gainOrbs(itDodeca, itOrbDash);
    gainOrbs(itGreenGrass, itOrbBull);
    gainOrbs(itBull, itOrbHorns);
    gainOrbs(itOrbYendor, itOrbMirror);
    gainOrbs(itLavaLily, itOrbLava);
    gainOrbs(itHunting, itOrbSide3);
    gainOrbs(itBlizzard, itOrbWinter);
    gainOrbs(itTerra, itOrbSide1);
    
    gainOrbs(itGlowCrystal, itOrbSide2);
    gainOrbs(itSwitch, itOrbPhasing);
    gainOrbs(itMagnet, itOrbMagnetism);
    gainOrbs(itRuins, itOrbSlaying);
    
    gainOrbs(itWest, itOrbGravity);
    gainOrbs(itVarTreasure, itOrbIntensity);
    gainOrbs(itBrownian, itOrbChoice);
    
    gainOrbs(itFrog, itOrbImpact);
    gainOrbs(itWet, itOrbPlague);
    gainOrbs(itEclectic, itOrbChaos);
    
    gainOrbs(itCursed, itOrbPurity);
    gainOrbs(itDice, itOrbLuck);

#if CAP_DAILY    
    daily::gifts();
#endif

    if(items[itOrbLove] && !items[itSavedPrincess]) items[itSavedPrincess] = 1;
    
    if(shmup::on) for(int i=0; i<ittypes; i++) {
      if(unlocked[i] && canbuy(eItem(i)) && isShmupLifeOrb(eItem(i))) {
        gainLife();
        usedOrb(eItem(i));
        }
      }

    items[itInventory] = points;
    }
  
  map<char, eItem> orbmap;
  string orbkeys = "zfwplSetsTaMIYgCcPOWAFydLGRUkouE.,bVNxDjJZnrvhBm!23456789@#$%()";
  
  typedef pair<int, int> pxy;
  vector<pxy> orbcoord;

  int sq(pxy p) {
    int zz = (2*p.first+p.second)*(2*p.first+p.second) + 3*p.second*p.second;
    zz *= 20; zz += abs(p.second);
    zz *= 20; zz += abs(p.first);
    zz *= 4; zz += (p.first>0)*2+(p.second>0);
    return zz;
    }

  bool plain;      

  eItem which;
  
  bool mirroring;
  
  EX const char* helptext = 
    "You are playing in the Orb Strategy Mode. Collecting treasure "
    "gives you access to magical Orb powers. In this mode, "
    "unlocking requirements are generally higher, and "
    "several quests and lands "
    "give you extremely powerful Orbs of the Mirror.\n";

  void evokeBeautyAt(cell *c) {
    forCellEx(c2, c)
      if(c2->monst && !isFriendly(c2->monst) && !isIvy(c2->monst)) {
        c2->stuntime += 3;
        checkStunKill(c2);
        }
    }
  
  void evokeOrb(eItem it) {
    if(it == itOrbFreedom)
      for(cell *pc: player_positions())
        checkFreedom(pc);
    
    if(it == itOrbBeauty) {
      for(cell *pc: player_positions()) 
          evokeBeautyAt(pc);
      if(items[itOrbEmpathy])
        for(cell *c: dcal) if(isFriendly(c->monst))
          evokeBeautyAt(c);
      }
    
    if(it == itOrbDigging) {
      forCellCM(c2, cwt.at) {
        earthFloor(c2);
        if(c2->wall == waCavewall && !c2->monst)
          c2->wall = waNone;
        }        
      }
    
    if(it == itOrbSword || it == itOrbSword2) {
      for(int i: player_indices()) {
        cwt.at = playerpos(i);
        multi::cpid = i;
        swordAttackStatic(it == itOrbSword2);
        }
      }
    }
  
  EX string osminfo(eItem orb) {
    string s = XLAT("Cost to use: %1/%2", its(usedup[orb] + 1), its(points));
    return s;
    }
  
  EX bool activating, view_help_only;

  /** \brief show the OSM Orb screen */
  EX void show() {
  
    multi::cpid = 0; /* just in case */
  
    if(unlocked[itOrbSword]) items[itOrbSword]++;
    if(unlocked[itOrbSword2]) items[itOrbSword2]++;
    cmode = sm::CENTER | sm::DARKEN;
    gamescreen();
    if(unlocked[itOrbSword]) items[itOrbSword]--;
    if(unlocked[itOrbSword2]) items[itOrbSword2]--;

    orbcoord.clear();
    for(int y=-3; y<=3; y++) for(int x=-5; x<=5; x++) if(x+y<=6 && x+y >= -6 && (x||y))
      orbcoord.emplace_back(x,y);
    sort(orbcoord.begin(), orbcoord.end(), [](pxy p1, pxy p2) {
      return sq(p1) < sq(p2); });
    
    ld rad = min(vid.xres, vid.yres) / 20;
    ld rad3 = int(rad * sqrt(3));
    
    compute();
    orbmap.clear();
    which = itNone;
        
    if(plain) {
      dialog::init(mirroring ? XLAT("mirror what?") : XLAT("inventory"), forecolor, 150, 100);
      dialog::start_list(2000, 2000);
      }

    int j = 0, oc = 6;

    if(1) {
    flat_model_enabler fme;

    for(int i=0; i<ittypes; i++) {
      eItem o = eItem(i);
      if(itemclass(o) == IC_ORB && !(classflag(o) & IF_CURSE)) {
        char c = orbkeys[j++];
        if(c == 0) println(hlog, "missing char for ", dnameof(o));
        if(unlocked[i]) {
          orbmap[c] = o;
          if(plain) 
            dialog::addSelItem(XLAT1(iinf[o].name), its(usedup[i] + 1), c);
          else {
            if(oc >= isize(orbcoord)) {
              println(hlog, "error: oc=", oc, " with only ", isize(orbcoord), " positions");
              continue;
              }
            auto pos = orbcoord[oc++];
            ld px = current_display->xcenter + 2*rad*pos.first + rad*pos.second;
            ld py = current_display->ycenter + pos.second * rad3;
            int icol = iinf[o].color;
            if(!canbuy(eItem(i))) icol = gradient(icol, 0, 0, .5, 1);
            bool gg = graphglyph(false);
            
            bool b = hypot(mousex-px, mousey-py) < rad;

            if(!hiliteclick) {
              if(gg) {
                initquickqueue();
                poly_outline = OUTLINE_DEFAULT;
                shiftmatrix V = atscreenpos(px, py, rad*2);
                drawItemType(o, NULL, V, icol, ticks/3 + i * 137, false);
                quickqueue();
                }

              string s = its(usedup[i] + 1);

              if(vid.orbmode < 2) {
                int tcol = canbuy(eItem(i)) ? darkenedby(icol, 1) : 0;

                displaystr(px, py, 2, gg?rad:rad*3/2, s, tcol, 8);
                }
              else {
                displayfr(px + rad/2, py + rad/2, 2, gg?rad:rad*3/2, s, dialog::dialogcolor_over(b), 8);
                }
              }

            if(b) {
              getcstat = c, 
              which = o;
              }
            }
          }
        }
      }
    }

    if(plain) {
      dialog::end_list();
      dialog::addBreak(50);
      dialog::addItem(XLAT("help"), SDLK_F1);
      dialog::addItem(XLAT("return to the game"), 'i');
      dialog::display();
      if(orbmap.count(getcstat)) which = orbmap[getcstat];
      }
    else {
      if(which == itNone) {
        displaystr(vid.xres/2, vid.fsize*2, 2, vid.fsize*2, XLAT("Which orb to use?"), 0xC0C0C0, 8);
        }
      else {
        int icol = iinf[which].color;
        displaystr(vid.xres/2, vid.fsize*2, 2, vid.fsize*2, XLAT1(iinf[which].name), icol, 8);
        
        if(mirroring)
          displaystr(vid.xres/2, vid.fsize*4, 2, vid.fsize, usedup[which] >= TESTMIRRORED ? XLAT("already mirrored") : XLAT("Uses to gain: %1", its(mirrorqty(which))), icol, 8);
        else {
          whichorbinfo = which;
          compute();
          
          displaystr(vid.xres/2, vid.fsize*4, 2, vid.fsize, orbinfoline, icol, 8);
                              
          if(extra != "")
            displaystr(vid.xres/2, vid.fsize*5, 2, vid.fsize, XLAT("Extras:")+extra, icol, 8);
          }

        if(unlocked[which]) {
          displaystr(vid.xres/2, vid.yres - vid.fsize*6, 2, vid.fsize, osminfo(which), icol, 8);
          }

#if !ISMOBILE
        string hot = XLAT1("Hotkey: "); hot += getcstat;
        displaystr(vid.xres/2, vid.yres - vid.fsize*5, 2, vid.fsize, hot, icol, 8);
#endif

        eLand pl = getPrizeLand();
        eOrbLandRelation olr = getOLR(which, pl);
        
        color_t col = 0;
        const char *fmsg = NULL;
        if(olr == olrDangerous) 
          col = 0xC00000,
          fmsg = "Using %the1 in %the2 sounds dangerous...";
        else if(olr == olrUseless) 
          col = 0xC00000,
          fmsg = "%The1 is mostly useless in %the2...";
        else if(olr == olrForbidden) 
          col = 0x804000,
          fmsg = "%The1 is forbidden in %the2 (disables some achievements)";
  
        if(fmsg)
          displaystr(vid.xres/2, vid.yres - vid.fsize*4, 2, vid.fsize, XLAT(fmsg, which, pl), col, 8);
  
        }
      }
    dialog::display_bottom_buttons(0, dialog::DB_BACK | dialog::DB_HELP | dialog::DB_PLAIN | (view_help_only ? dialog::DB_EXPLAIN : dialog::DB_ACTIVATE));
    mouseovers = "";
    keyhandler = [] (int sym, int uni) {
      if(plain) dialog::handleNavigation(sym, uni);
      
      if(orbmap.count(uni)) {
        eItem orb = orbmap[uni];
        if(view_help_only) {
          gotoHelp(generateHelpForItem(orb));
          }
        else if(!canbuy(orb)) ;
        else if(orb == itOrbMirror) {
          mirroring = !mirroring;
          // an amusing message
          if(canbuy(itOrbMirror) && !mirroring) 
            addMessage(XLAT("You mirror %the1.", orb));
          if(mirroring) {
            bool next = false;
            forCellEx(c2, cwt.at) if(c2->wall == waMirror || c2->wall == waCloud || c2->wall == waMirrorWall)
              next = true;
            if(!next) {
              addMessage(XLAT("You need to stand next to a magic mirror or cloud to use %the1.", itOrbMirror));
              mirroring = false;
              }
            }
          }
        else if(mirroring) {
          if(usedup[orb] >= TESTMIRRORED) {
            addMessage(XLAT("Each orb type can be mirrored only once."));
            mirroring = false;
            }
          else if(unlocked[orb]) {
            usedup[orb]--;
            usedOrb(itOrbMirror);
            addMessage(XLAT("You mirror %the1.", orb));
            mirroring = false;
            }
          else mirroring = false;
          }
        else if((isHaunted(cwt.at->land) || cwt.at->land == laDungeon) && orb == itOrbSafety) {
          addMessage(XLAT("This would only move you deeper into the trap!"));
          }
        else {
          eItem it = cwt.at->item; 
          cwt.at->item = orbmap[uni];
          inv::activating = true;
          collectItem(cwt.at, cwt.at, true);
          inv::activating = false;
          addMessage(XLAT("You activate %the1.", orbmap[uni]));
          if(!cwt.at->item)
            {
              usedOrb(orbmap[uni]);
            }
          if(getOLR(it, getPrizeLand()) == olrForbidden)
            usedForbidden = true;
          cwt.at->item = it;
          evokeOrb(orbmap[uni]);
          checkmove(false);
          popScreenAll();
          }
        }
      
      else if(uni == '1') plain = !plain;
      else if(uni == '2') view_help_only = !view_help_only;
      else if(sym == SDLK_F1) 
        gotoHelp(which ? generateHelpForItem(which) : XLAT(helptext));
      else if(doexiton(sym, uni)) {
        if(mirroring) mirroring = false;
        popScreen();
        }
      };
    }

#if CAP_SAVE
  EX void applyBox(eItem it) {
    scores::applyBoxNum(inv::usedup[it], "@inv-" + dnameof(it));
    }
#endif
  
  EX int incheck;
  
  EX void check(int delta) {
    incheck += delta;
    for(int i=0; i<ittypes; i++) {
      eItem it = eItem(i);
      if(itemclass(it) == IC_ORB)
        items[it] += delta * (canbuy(it) ? 1 : 0) * orbcharges(it);
      }
    }

#endif

#if !CAP_INV
EX always_false on, activating;
#endif
EX }

}
