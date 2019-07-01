# Twilight Princess Randomizer

#### Disclaimer

> This project is based on Piston's [ttyd-tools](https://github.com/PistonMiner/ttyd-tools) and ported to Twilight Princess by [Zephiles](https://github.com//Zephiles)
> 
### Build Requirements
* [DevkitPPC](https://devkitpro.org/)
* [GC-DevTools](/zsrtp/GC-DevTools)

> After downloading the DevTools you want to adjust some files for the  **Makefile** to match your **elf2rel** and **gcipack** path(s)

**Linux/Unix:**

`$ sudo cp /path/to/elf2rel /usr/bin/elf2rel`

`$ echo 'python3 /path/to/gcipack.py "$@"' > /usr/bin/gcipack && sudo chmod +x /usr/bin/gcipack`

In this case the **Makefile** does not need to be changed, however if you don't want to add the binaries/scripts to your *PATH* or *bin*  you should edit the Makefile (see Windows variant)

**Windows**
* Open the **Makefile** for edit
* Change line 12 from `export ELF2REL := elf2rel` to `export ELF2REL := /path/to/elf2rel.exe` (or `/path/to/elf2rel` on linux)
* Change line 13 from `export GCIPACK := gcipack` to `export GCIPACK := python3 /path/to/gcipack.py`

> *It is suggested to create a `bin` folder inside the Randomizer and copy the binary/script (`elf2rel`, `gcipack.py`) inside there - then use `bin/elf2rel[.exe]` and `bin/gcipack.py` as paths*

### Usage
WiP
