# Twilight Princess REL base
[Homepage](https://rando.tpspeed.run) | [Development Information](https://dev.tpspeed.run) | [Mirror](https://git.aecx.cc) | [Discord](https://discord.tpspeed.run)

#### Disclaimer
> In early 2020, a new repository called [Randomizer](https://github.com/zsrtp/Randomizer) was made for this randomizer due to massive code and structural changes that are planned. At the moment, it is empty.

> This project is based on Piston's [ttyd-tools](https://github.com/PistonMiner/ttyd-tools) and ported to Twilight Princess by [Zephiles](https://github.com//Zephiles)

### Info
This is template code without any game changing logic whatsoever.
It randomizes every item Link gets then turns towards the camera with the item floating above his hands (excluding getting an item for the first time on the ground).
This means these items are not randomized in this version:
* Master Sword
* Shadow Crystal
* Horse Call
* Master Sword infused with light
* Recharged Dominion Rod
* All 7 hidden skills
* All 5 scents
* Shop ammo that doesn't trigger a cutscene
* Floor rupees/arrows/seeds
* Scoopable bottle contents (might be possible in an update from the [official version](https://github.com/zsrtp/Randomizer) )
* Fished items (might be possible in an update from the [official version](https://github.com/zsrtp/Randomizer) )

### What is planned for later versions
* Implementing chests in place of non randomizable unique items
* Making shop ammo that doesn't trigger a cutscene randomizable
* Skips (idk Lunar will always find something more to skip)
* Text edits

### If you think an item check wasn't randomized properly:
1. Get the item check in-game
2. Open the in-game console (R+Z) before getting another item
3. Go to page 3 (named "Debug Info")
	1. If the lines "Function:", "Source:" and "Dest:" didn't change: it means that we can't randomize that item at the moment since we don't know the trigger that gets called when this item is gotten
	2. If the line "Source:" does not contain some "->" and the line "Dest:" says: "No Replacement found for this source", it means that item check wasn't found, so:
		1. It could be abscent from our code
		2. It could be in the code but with some incorrect data
	3. If the line "Source:" looks someting like "F_SP08->5->7" and the line "Dest:" says: "No replacement here.", it means that the item check was found and already collected, so:
		1. The trigger for getting the item might get fired more than once
		2. There could be a problem with the logic
	4. If the line "Source:" and the line "Dest:" look someting like "F_SP08->5->7", then the item check was randomized properly
4. If any of these cases happen you must take note of:
	1. page 3 "Debug Info", the lines "Function:", "Source:", "Dest:", "Active Seed:" and "Checksum:"
	2. page 4 "Item Search", the lines "MetadataID:", "MetadataX:", "MetadataY:" and "MetadataZ:"
	3. page 5 "Game Info", the lines "CurrentStage:", "NextStage:" and "NextRoom:"
	4. top right, the randomizer versions
5. After this, feel free to mention it in the [Discord](https://discord.tpspeed.run), where we will look into this item check
6. You can also look into the [newer releases](https://github.com/zsrtp/GC-Randomizer/releases) to see if that problem was already taken care of

### Build Requirements
* [DevkitPPC](https://devkitpro.org/)
* [GC-DevTools](https://github.com/zsrtp/GC-DevTools)

> After downloading the DevTools you want to adjust some files for the  **Makefile** to match your **elf2rel** and **gcipack** path(s)

**Linux/Unix:**

`$ sudo cp /path/to/elf2rel /usr/bin/elf2rel`

`$ sudo echo 'python3 /path/to/gcipack.py "$@"' > /usr/bin/gcipack && sudo chmod +x /usr/bin/gcipack`

In this case the **Makefile** does not need to be changed, however if you don't want to add the binaries/scripts to your *PATH* or *bin*  you should edit the Makefile (see Windows variant)

**Windows**
* Open the **Makefile** for edit
* Change line 12 from `export ELF2REL := elf2rel` to `export ELF2REL := /path/to/elf2rel.exe` (or `/path/to/elf2rel` on linux)
* Change line 13 from `export GCIPACK := gcipack` to `export GCIPACK := python3 /path/to/gcipack.py`

> *It is suggested to create a `bin` folder inside the Randomizer and copy the binary/script (`elf2rel`, `gcipack.py`) inside there - then use `bin/elf2rel[.exe]` and `bin/gcipack.py` as paths*

### Credits and special thanks to
* AECX for the first versions of the randomizer
* Piston for TTYD-Tools
* Zephiles for porting them to TP and helping to set them up properly aswell as hours of testing, debugging and providing help
* Taka for helping *a lot* with testing and gathering meta-data for chests
* Skyreon for also helping with REL testing
* Hornlitz for writing down all chests + some information on how to access them
* Jacquaid for his TPHD rando logic and general information
* Dragonbane for providing inifinite amounts of information and being a constant source of Game knowledge
* Everyone who contributed to researching the game and/or was involved in the Ultimate TP Spreadsheet
