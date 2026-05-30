#include "archipelago.hpp"

void ap::init::initItemByID(){
  ap::itemByID[0X00]=ap::getItemByName("Ice Diamond");
  ap::itemByID[0X01]=ap::getItemByName("Gold");
  ap::itemByID[0X02]=ap::getItemByName("Spice");
  ap::itemByID[0X03]=ap::getItemByName("Ruby");
  ap::itemByID[0X04]=ap::getItemByName("Elixir of Life");
  ap::itemByID[0X05]=ap::getItemByName("Shard");
  ap::itemByID[0X06]=ap::getItemByName("Necromancer's Totem");
  ap::itemByID[0X07]=ap::getItemByName("Demon Daisy");
  ap::itemByID[0X08]=ap::getItemByName("Statue of Cthulhu");
  ap::itemByID[0X09]=ap::getItemByName("Phoenix Feather");
  ap::itemByID[0X0A]=ap::getItemByName("Ice Sapphire");
  ap::itemByID[0X0B]=ap::getItemByName("Fern Flower");
  ap::itemByID[0X0C]=ap::getItemByName("Wine");
  ap::itemByID[0X0D]=ap::getItemByName("Silver");
  ap::itemByID[0X0E]=ap::getItemByName("Royal Jelly");
  ap::itemByID[0X0F]=ap::getItemByName("Emerald");
  ap::itemByID[0X10]=ap::getItemByName("Powerstone");
  ap::itemByID[0X11]=ap::getItemByName("Holy Grail");
  ap::itemByID[0X12]=ap::getItemByName("Grimoire");
  ap::itemByID[0X13]=ap::getItemByName("Pirate Treasure");
  ap::itemByID[0X14]=ap::getItemByName("Red Gem");
  ap::itemByID[0X15]=ap::getItemByName("Bomberbird Egg");
  ap::itemByID[0X16]=ap::getItemByName("Amber");
  ap::itemByID[0X17]=ap::getItemByName("Pearl");
  ap::itemByID[0X18]=ap::getItemByName("Hypersian Rug");
  ap::itemByID[0X19]=ap::getItemByName("Garnet");
  ap::itemByID[0X1A]=ap::getItemByName("Ivory Figurine");
  ap::itemByID[0X1B]=ap::getItemByName("Onyx");
  ap::itemByID[0X1C]=ap::getItemByName("Elemental Gem");
  ap::itemByID[0X1D]=ap::getItemByName("Fulgurite");
  ap::itemByID[0X1E]=ap::getItemByName("Mutant Sapling");
  ap::itemByID[0X1F]=ap::getItemByName("Mutant Fruit");
  ap::itemByID[0X20]=ap::getItemByName("Black Lotus");
  ap::itemByID[0X21]=ap::getItemByName("White Dove Feather");
  ap::itemByID[0X22]=ap::getItemByName("Thornless Rose");
  ap::itemByID[0X23]=ap::getItemByName("Coral");
  ap::itemByID[0X24]=ap::getItemByName("Baby Tortoise");
  ap::itemByID[0X25]=ap::getItemByName("Apple");
  ap::itemByID[0X26]=ap::getItemByName("Dragon Scale");
  ap::itemByID[0X27]=ap::getItemByName("Sunken Treasure");
  ap::itemByID[0X28]=ap::getItemByName("Ancient Jewelry");
  ap::itemByID[0X29]=ap::getItemByName("Golden Egg");
  ap::itemByID[0X2A]=ap::getItemByName("Slime Mold");
  ap::itemByID[0X2B]=ap::getItemByName("Amethyst");
  ap::itemByID[0X2C]=ap::getItemByName("Dodecahedron");
  ap::itemByID[0X2D]=ap::getItemByName("Green Grass");
  ap::itemByID[0X2E]=ap::getItemByName("Spinel");
  ap::itemByID[0X2F]=ap::getItemByName("Lava Lily");
  ap::itemByID[0X30]=ap::getItemByName("Turquoise");
  ap::itemByID[0X31]=ap::getItemByName("Forgotten Relic");
  ap::itemByID[0X32]=ap::getItemByName("Ancient Weapon");
  ap::itemByID[0X33]=ap::getItemByName("Chrysoberyl");
  ap::itemByID[0X34]=ap::getItemByName("Tasty Jelly");
  ap::itemByID[0X35]=ap::getItemByName("Tiger's Eye");
  ap::itemByID[0X36]=ap::getItemByName("Meteorite");
  ap::itemByID[0X37]=ap::getItemByName("Torbernite");
  ap::itemByID[0X38]=ap::getItemByName("Water Lily");
  ap::itemByID[0X39]=ap::getItemByName("Gold Ball");
  ap::itemByID[0X3A]=ap::getItemByName("Lazurite Figurine");
  ap::itemByID[0X3B]=ap::getItemByName("Capon Stone");
  ap::itemByID[0X3C]=ap::getItemByName("Crystal Die");

//nonstandard
  ap::itemByID[0X50]=ap::getItemByName("Bounty");
  ap::itemByID[0X51]=ap::getItemByName("Treat");
  ap::itemByID[0X52]=ap::getItemByName("Glowing Crystal");
  ap::itemByID[0X53]=ap::getItemByName("Snake Oil");
  ap::itemByID[0X54]=ap::getItemByName("Sea Glass");
  ap::itemByID[0X55]=ap::getItemByName("Fuel");
}

void ap::init::initOrbByID(){
  ap::orbByID[0X00]=ap::getItemByName("Orb of Flash");
  ap::orbByID[0X01]=ap::getItemByName("Orb of Life");
  ap::orbByID[0X02]=ap::getItemByName("Orb of Shielding");
  ap::orbByID[0X03]=ap::getItemByName("Orb of Storms");
  ap::orbByID[0X04]=ap::getItemByName("Orb of Speed");
  ap::orbByID[0X05]=ap::getItemByName("Orb of the Mirror");
  ap::orbByID[0X06]=ap::getItemByName("Dead Orb");
  ap::orbByID[0X07]=ap::getItemByName("Orb of Yendor");
  ap::orbByID[0X08]=ap::getItemByName("Orb of Teleport");
  ap::orbByID[0X09]=ap::getItemByName("Orb of Safety");
  ap::orbByID[0X0A]=ap::getItemByName("Orb of Change");
  ap::orbByID[0X0B]=ap::getItemByName("Orb of Thorns");
  ap::orbByID[0X0C]=ap::getItemByName("Orb of Aether");
  ap::orbByID[0X0D]=ap::getItemByName("Orb of Earth");
  ap::orbByID[0X0E]=ap::getItemByName("Orb of Invisibility");
  ap::orbByID[0X0F]=ap::getItemByName("Orb of the Mind");
  ap::orbByID[0X10]=ap::getItemByName("Orb of Fire");
  ap::orbByID[0X11]=ap::getItemByName("Orb of Trickery");
  ap::orbByID[0X12]=ap::getItemByName("Orb of the Dragon");
  ap::orbByID[0X13]=ap::getItemByName("Orb of Time");
  ap::orbByID[0X14]=ap::getItemByName("Orb of Space");
  ap::orbByID[0X15]=ap::getItemByName("Orb of Friendship");
  ap::orbByID[0X16]=ap::getItemByName("Orb of Empathy");
  ap::orbByID[0X17]=ap::getItemByName("Orb of Water");
  ap::orbByID[0X18]=ap::getItemByName("Orb of Discord");
  ap::orbByID[0X19]=ap::getItemByName("Orb of the Fish");
  ap::orbByID[0X1A]=ap::getItemByName("Orb of Matter");
  ap::orbByID[0X1B]=ap::getItemByName("Orb of the Frog");
  ap::orbByID[0X1C]=ap::getItemByName("Orb of Summoning");
  ap::orbByID[0X1D]=ap::getItemByName("Orb of Stunning");
  ap::orbByID[0X1E]=ap::getItemByName("Orb of the Woods");
  ap::orbByID[0X1F]=ap::getItemByName("Orb of Freedom");
  ap::orbByID[0X20]=ap::getItemByName("Orb of Undeath");
  ap::orbByID[0X21]=ap::getItemByName("Orb of Air");
  ap::orbByID[0X22]=ap::getItemByName("Orb of Beauty");
  ap::orbByID[0X23]=ap::getItemByName("Orb of the Warp");
  ap::orbByID[0X24]=ap::getItemByName("Orb of the Shell");
  ap::orbByID[0X25]=ap::getItemByName("Orb of Energy");
  ap::orbByID[0X26]=ap::getItemByName("Orb of Domination");
  ap::orbByID[0X27]=ap::getItemByName("Orb of the Sword");
  ap::orbByID[0X28]=ap::getItemByName("Orb of the Sword II");
  ap::orbByID[0X29]=ap::getItemByName("Orb of the Stone");
  ap::orbByID[0X2A]=ap::getItemByName("Orb of Recall");
  ap::orbByID[0X2B]=ap::getItemByName("Orb of Nature");
  ap::orbByID[0X2C]=ap::getItemByName("Orb of Vaulting");
  ap::orbByID[0X2D]=ap::getItemByName("Orb of the Bull");
  ap::orbByID[0X2E]=ap::getItemByName("Orb of Horns");
  ap::orbByID[0X2F]=ap::getItemByName("Orb of Lava");
  ap::orbByID[0X30]=ap::getItemByName("Orb of Ferocity");
  ap::orbByID[0X31]=ap::getItemByName("Orb of Winter");
  ap::orbByID[0X32]=ap::getItemByName("Orb of Slashing");
  ap::orbByID[0X33]=ap::getItemByName("Orb of Slaying");
  ap::orbByID[0X34]=ap::getItemByName("Orb of Phasing");
  ap::orbByID[0X35]=ap::getItemByName("Orb of Choice");
  ap::orbByID[0X36]=ap::getItemByName("Orb of Gravity");
  ap::orbByID[0X37]=ap::getItemByName("Orb of Intensity");
  ap::orbByID[0X38]=ap::getItemByName("Orb of Plague");
  ap::orbByID[0X39]=ap::getItemByName("Orb of Impact");
  ap::orbByID[0X3A]=ap::getItemByName("Orb of Chaos");
  ap::orbByID[0X3B]=ap::getItemByName("Orb of Purity");
  ap::orbByID[0X3C]=ap::getItemByName("Orb of Luck");

//nonstandard
//  ap::orbByID[0X50]=ap::getItemByName("Bounty");
//  ap::orbByID[0X51]=ap::getItemByName("Treat");
//  ap::orbByID[0X52]=ap::getItemByName("Glowing Crystal");
//  ap::orbByID[0X53]=ap::getItemByName("Snake Oil");
//  ap::orbByID[0X54]=ap::getItemByName("Sea Glass");
//  ap::orbByID[0X55]=ap::getItemByName("Fuel");
}

constexpr unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

int ap::getLocationID(eItem treas, ap::progressCheck prog, bool extra){
  if(prog == ap::progressCheck::notingame || prog == ap::progressCheck::locked) return -1;
  int progBaseID = 0X100 * (((int) prog) - 1) + 0X300 * (int) extra;
  switch(str2int(iinf[treas].name)){
  case str2int("Ice Diamond"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x000;
  case str2int("Gold"):                return  HYPERROGUE_BASE_ID + progBaseID + 0x001;
  case str2int("Spice"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x002;
  case str2int("Ruby"):                return  HYPERROGUE_BASE_ID + progBaseID + 0x003;
  case str2int("Elixir of Life"):      return  HYPERROGUE_BASE_ID + progBaseID + 0x004;
  case str2int("Shard"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x005;
  case str2int("Necromancer's Totem"): return  HYPERROGUE_BASE_ID + progBaseID + 0x006;
  case str2int("Demon Daisy"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x007;
  case str2int("Statue of Cthulhu"):   return  HYPERROGUE_BASE_ID + progBaseID + 0x008;
  case str2int("Phoenix Feather"):     return  HYPERROGUE_BASE_ID + progBaseID + 0x009;
  case str2int("Ice Sapphire"):        return  HYPERROGUE_BASE_ID + progBaseID + 0x00A;
  case str2int("Fern Flower"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x00B;
  case str2int("Wine"):                return  HYPERROGUE_BASE_ID + progBaseID + 0x00C;
  case str2int("Silver"):              return  HYPERROGUE_BASE_ID + progBaseID + 0x00D;
  case str2int("Royal Jelly"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x00E;
  case str2int("Emerald"):             return  HYPERROGUE_BASE_ID + progBaseID + 0x00F;
  case str2int("Powerstone"):          return  HYPERROGUE_BASE_ID + progBaseID + 0x010;
  case str2int("Holy Grail"):          return  HYPERROGUE_BASE_ID + progBaseID + 0x011;
  case str2int("Grimoire"):            return  HYPERROGUE_BASE_ID + progBaseID + 0x012;
  case str2int("Pirate Treasure"):     return  HYPERROGUE_BASE_ID + progBaseID + 0x013;
  case str2int("Red Gem"):             return  HYPERROGUE_BASE_ID + progBaseID + 0x014;
  case str2int("Bomberbird Egg"):      return  HYPERROGUE_BASE_ID + progBaseID + 0x015;
  case str2int("Amber"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x016;
  case str2int("Pearl"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x017;
  case str2int("Hypersian Rug"):       return  HYPERROGUE_BASE_ID + progBaseID + 0x018;
  case str2int("Garnet"):              return  HYPERROGUE_BASE_ID + progBaseID + 0x019;
  case str2int("Ivory Figurine"):      return  HYPERROGUE_BASE_ID + progBaseID + 0x01A;
  case str2int("Onyx"):                return  HYPERROGUE_BASE_ID + progBaseID + 0x01B;
  case str2int("Elemental Gem"):       return  HYPERROGUE_BASE_ID + progBaseID + 0x01C;
  case str2int("Fulgurite"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x01D;
  case str2int("Mutant Sapling"):      return  HYPERROGUE_BASE_ID + progBaseID + 0x01E;
  case str2int("Mutant Fruit"):        return  HYPERROGUE_BASE_ID + progBaseID + 0x01F;
  case str2int("Black Lotus"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x020;
  case str2int("White Dove Feather"):  return  HYPERROGUE_BASE_ID + progBaseID + 0x021;
  case str2int("Thornless Rose"):      return  HYPERROGUE_BASE_ID + progBaseID + 0x022;
  case str2int("Coral"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x023;
  case str2int("Baby Tortoise"):       return  HYPERROGUE_BASE_ID + progBaseID + 0x024;
  case str2int("Apple"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x025;
  case str2int("Dragon Scale"):        return  HYPERROGUE_BASE_ID + progBaseID + 0x026;
  case str2int("Sunken Treasure"):     return  HYPERROGUE_BASE_ID + progBaseID + 0x027;
  case str2int("Ancient Jewelry"):     return  HYPERROGUE_BASE_ID + progBaseID + 0x028;
  case str2int("Golden Egg"):          return  HYPERROGUE_BASE_ID + progBaseID + 0x029;
  case str2int("Slime Mold"):          return  HYPERROGUE_BASE_ID + progBaseID + 0x02A;
  case str2int("Amethyst"):            return  HYPERROGUE_BASE_ID + progBaseID + 0x02B;
  case str2int("Dodecahedron"):        return  HYPERROGUE_BASE_ID + progBaseID + 0x02C;
  case str2int("Green Grass"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x02D;
  case str2int("Spinel"):              return  HYPERROGUE_BASE_ID + progBaseID + 0x02E;
  case str2int("Lava Lily"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x02F;
  case str2int("Turquoise"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x030;
  case str2int("Forgotten Relic"):     return  HYPERROGUE_BASE_ID + progBaseID + 0x031;
  case str2int("Ancient Weapon"):      return  HYPERROGUE_BASE_ID + progBaseID + 0x032;
  case str2int("Chrysoberyl"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x033;
  case str2int("Tasty Jelly"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x034;
  case str2int("Tiger's Eye"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x035;
  case str2int("Meteorite"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x036;
  case str2int("Torbernite"):          return  HYPERROGUE_BASE_ID + progBaseID + 0x037;
  case str2int("Water Lily"):          return  HYPERROGUE_BASE_ID + progBaseID + 0x038;
  case str2int("Gold Ball"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x039;
  case str2int("Lazurite Figurine"):   return  HYPERROGUE_BASE_ID + progBaseID + 0x03A;
  case str2int("Capon Stone"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x03B;
  case str2int("Crystal Die"):         return  HYPERROGUE_BASE_ID + progBaseID + 0x03C;
  case str2int("Bounty"):              return  HYPERROGUE_BASE_ID + progBaseID + 0x050;
  case str2int("Treat"):               return  HYPERROGUE_BASE_ID + progBaseID + 0x051;
  case str2int("Glowing Crystal"):     return  HYPERROGUE_BASE_ID + progBaseID + 0x052;
  case str2int("Snake Oil"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x053;
  case str2int("Sea Glass"):           return  HYPERROGUE_BASE_ID + progBaseID + 0x054;
  case str2int("Fuel"):                return  HYPERROGUE_BASE_ID + progBaseID + 0x055;
  default: return -1;
  }
  
}

namespace ap{
std::vector<std::string> invalidStartingLandNames({
  "Kraken Depths",
  "Whirlpool",
  "Temple of Cthulhu",
  "Ivory Tower",
  "Yendorian Forest",
  "Free Fall",
  "Dungeon",
  "Lost Mountain",
  "Haunted Woods",
  "Clearing",
  "Camelot",
  "Crossroads V",
  "None"
});
}