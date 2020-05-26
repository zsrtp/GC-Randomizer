# Twilight Princess REL base
[Homepage](https://rando.zeldatp.net) | [Mirror](https://git.zeldatp.net) | [Discord](https://discord.zeldatp.net)

#### Disclaimer
> The new Repository will just be [Randomizer](https://github.com/zsrtp/Randomizer) due to massive code and structural changes
> Currently this is still active!

> This project is based on Piston's [ttyd-tools](https://github.com/PistonMiner/ttyd-tools) and ported to Twilight Princess by [Zephiles](https://github.com//Zephiles)

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
* dragonbane for providing inifinite amounts of information and being a constant source of Game knowledge
* Everyone who contributed to researching the game and/or was involved in the Ultimate TP Spreadsheet
