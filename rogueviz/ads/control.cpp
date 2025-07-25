namespace hr {

namespace ads_game {

enum pcmds_extra {
  pcPause=9, pcDisplayTimes=10, pcSwitchSpin=11, pcMenu=12,
  pcPauseFuture=13, pcPausePast=14, pcPauseMoveSwitch=15
  };

vector<string> move_names = { "", "", "", "", "acc down", "acc left", "acc up", "acc right", "fire", "pause", "display times", "switch spin", "menu", "[paused] future", "[paused] past", "[paused] move switch" };

void fire() {
  if(!pdata.ammo) return;
  playSound(nullptr, "fire");
  pdata.ammo--;
  auto c = vctr;

  ads_matrix S0 = ads_inverse(current * vctrV) * spin(ang*degree);

  ads_matrix S1 = S0 * lorentz(0, 2, ads_missile_rapidity);
  
  auto& ro = ci_at[c].rocks;
  auto r = std::make_unique<ads_object> (oMissile, c, S1, rsrc_color[rtAmmo]);
  r->shape = &shape_missile;
  r->life_start = 0;
  r->life_end = M_PI;
  r->shot_at = ship_pt;

  ads_matrix Scell(Id, 0);    
  cell *lcell = hybrid::get_at(vctr, 0);
  auto wcell = make_pair(vctr, 0);
  
  int steps = 0;

  compute_life(lcell, unshift(r->at), [&] (cell *c1, ld t) {
    if(true) for(int i=0; i<lcell->type; i++) {
      auto lcell1 = lcell->cmove(i);
      auto wcell1 = hybrid::get_where(lcell1);
      if(wcell1.first == c1) {
        Scell = Scell * currentmap->adj(lcell, i);
        optimize_shift(Scell);
        lcell = lcell1;
        wcell = wcell1;
        adjust_to_zero(Scell, wcell, cgi.plevel);
        steps++;
        lcell = hybrid::get_at(wcell.first, 0);
        break;
        }
      }
    if(true) if(wcell.first != c1) {
      println(hlog, "warning: got lost after ", steps, " steps");
      println(hlog, wcell);
      println(hlog, c1);
      println(hlog, "their distance is ", PIU(celldistance(wcell.first, c1)));
      return true;
      }
    auto& ci = ci_at[c1];
    hybrid::in_underlying_geometry([&] {
      gen_terrain(c1, ci);
      });
    if(among(ci.type, wtSolid, wtDestructible, wtBarrier)) {
      r->life_end = t;

      auto Scell_inv = ads_inverse(Scell);
      Scell_inv = Scell_inv * r->at;
      Scell_inv = Scell_inv * ads_matrix(Id, t);
      optimize_shift(Scell_inv);

      auto X = ads_inverse(Scell);
      X = X * (r->at * ads_matrix(Id, t));
      optimize_shift(X);

      ads_matrix prel = ads_inverse(S0) * r->at * ads_matrix(Id, t);

      println(hlog, "crashed: proper time = ", t/TAU, " wall time = ", Scell_inv.shift / TAU, " player time = ", (prel.shift+ship_pt) / TAU, " start = ", ship_pt / TAU);
      if(abs(X.shift - Scell_inv.shift) > .2) {
        println(hlog, "INTRANSITIVITY ERROR! ", X.shift, " vs ", Scell_inv.shift);
        exit(1);
        }

      return true;
      }
    return false;
    });
  ro.emplace_back(std::move(r));
  }

bool handleKey(int sym, int uni) {
  if(cmode & sm::NORMAL) {
    int* t = multi::scfg_default.keyaction;
    if(sym >= 0 && sym < multi::SCANCODES && t[sym] >= 16 && t[sym] < 32) return true;
    if(sym == 'v') pushScreen(game_menu);
    if(sym == SDLK_ESCAPE) pushScreen(game_menu);
    if(sym == SDLK_F1) {
      if(help == "@") help = get_main_help();
      gotoHelp(help);
      }
    return true;
    }
  return false;
  }

void apply_lorentz(transmatrix lor) {
  current = ads_matrix(lor, 0) * current;
  }

ld read_movement() {

  auto& axes = multi::axes_for(0);

  ld mdx = axes[0]/30000.;
  ld mdy = axes[1]/30000.;
  #if CAP_VR
  if(vrhr::active()) {
    mdy -= vrhr::vrgo_y;
    mdx += vrhr::vrgo_x;
    }
  #endif
  if(mdx || mdy) {
    ang = atan2(mdy, mdx) / degree;
    return hypot(mdx, mdy);
    }

  auto& act = multi::action_states[1];
  bool left = act[multi::pcMoveLeft];
  bool right = act[multi::pcMoveRight];
  bool up = act[multi::pcMoveUp];
  bool down = act[multi::pcMoveDown];

  int clicks = (left?1:0) + (right?1:0) + (up?1:0) + (down?1:0);

  if(left && right) left = right = false;
  if(up && down) up = down = false;

  if(left) ang = 180;
  if(right) ang = 0;
  if(up) ang = 90;
  if(down) ang = 270;

  if(left && up) ang = 135;
  if(left && down) ang = 225;
  if(right && up) ang = 45;
  if(right && down) ang = 315;

  ld mul = clicks ? 1 : 0;
  if(clicks > 2) mul *= .3;
  if(!paused) {
    if(game_over || pdata.fuel < 0) mul = 0;
    }

  return mul;
  }

bool ads_turn(int idelta) {
  multi::handleInput(idelta, multi::scfg_default);
  ld delta = idelta / 1000.;

  if(!(cmode & sm::NORMAL)) return false;

  hybrid::in_actual([&] {

  handle_crashes();
  if(no_param_change && !all_params_default()) no_param_change = false;

  auto& act = multi::action_states[1];

  if(act[multi::pcFire].pressed() && !paused) fire();
  if(act[pcPause].pressed()) switch_pause();
  if(act[pcDisplayTimes].pressed()) view_proper_times = !view_proper_times;
  if(act[pcSwitchSpin].pressed()) auto_rotate = !auto_rotate;
  if(act[pcMenu].pressed()) pushScreen(game_menu);

  if(auto_angle) pconf.mori().get() = spin(ang) * pconf.mori().get();

  if(true) {
    
    /* proper time passed */
    ld pt = delta * ads_simspeed;
    
    ld mul = read_movement();
    ld dv = pt * ads_accel * mul;


    if(paused && act[pcPauseMoveSwitch]) {
      current = ads_matrix(spin(ang*degree) * xpush(mul*delta*-pause_speed) * spin(-ang*degree), 0) * current;
      }
    else
      apply_lorentz(spin(ang*degree) * lorentz(0, 2, -dv) * spin(-ang*degree));
    
    if(!paused) {
      pdata.fuel -= dv;
      gen_particles(rpoisson(dv*fuel_particle_qty), vctr, ads_inverse(current * vctrV) * spin(ang*degree+M_PI) * twist::uxpush(0.06 * get_scale()), rsrc_color[rtFuel], fuel_particle_rapidity, fuel_particle_life, 0.02);
      }

    ld tc = 0;
    if(!paused) tc = pt;
    else if(act[pcPauseFuture]) tc = pt;
    else if(act[pcPausePast]) tc = -pt;

    if(!paused && !game_over) {
      shipstate ss;
      ss.at = ads_inverse(current * vctrV) * spin((ang+90)*degree);
      ss.ang = ang;
      ss.current = current;
      ss.start = ship_pt;
      ss.duration = pt;
      ss.vctrV = vctrV;
      ss.vctr = vctr;
      history.emplace_back(ss);
      ci_at[vctr].shipstates.emplace_back(ss);
      }

    current.T = cspin(3, 2, tc) * current.T;
    
    optimize_shift(current);    
    hassert(eqmatrix(chg_shift(current.shift) * current.T, unshift(current)));
    
    if(auto_rotate)
      current.T = cspin(1, 0, tc) * current.T;
    else if(!paused && !keep_ship_angle)
      ang += tc / degree;

    if(!paused) {
      ship_pt += pt;
      pdata.oxygen -= pt;
      if(pdata.oxygen < 0) {
        pdata.oxygen = 0;
        game_over_with_message("suffocated");
        }
      }
    else view_pt += tc;
    }

  if(auto_angle) pconf.mori().get() = spin(-ang) * pconf.mori().get();
  
  fixmatrix_ads(current.T);
  fixmatrix_ads(new_vctrV.T);
  });

  return true;
  }

}}
