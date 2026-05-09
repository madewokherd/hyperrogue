# About
This is a randomizer for the game [Hyperrogue](https://store.steampowered.com/app/342610/HyperRogue/).

The randomizer is fully playable but still lacks some documentation and testing.


## What does it do?
To send out a check, obtain 10/25/50 treasures (depending on settings) in a land or unlock a land for the first time.
Checks you obtain are Progressive: For each land, you first obtain the unlock, then its Orb in that land, then its Orb spawning globally, and lastly (Depending on settings) the land is removed from the list of lands required for the Hyperstone quest.


### Setup for Windows
1. Install [MSYS2](https://www.msys2.org/). Launch MSYS2 UCRT64 and paste the following command:
pacman -S mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-SDL mingw-w64-ucrt-x86_64-SDL_mixer mingw-w64-ucrt-x86_64-SDL_ttf mingw-w64-ucrt-x86_64-SDL_gfx mingw-w64-ucrt-x86_64-glew

2. Download the .zip file from below and extract it. 

3. In the extracted folder, open apsettings.json and enter your ip and slotname.

4. Navigate MSYS2 UCRT64 to the extracted folder by running:
cd /Path/to/Your/Folder

5. Run:
./hyperrogue.exe

6. Enjoy!


### Setup for Linux with apt-get
1. Run:
sudo apt-get install make g++ libsdl1.2-dev libsdl-ttf2.0-dev libsdl-gfx1.2-dev libsdl-mixer1.2-dev libglew-dev libpng-dev zlib1g-dev

2. Download the .zip file from below and extract it. 

3. In the extracted folder, open apsettings.json and enter your ip and slotname.

4. In the extracted folder, run ./hyperrogue.

5. Enjoy!