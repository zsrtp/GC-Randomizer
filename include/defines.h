#pragma once

#include <cstddef>

using std::size_t;

// Data types
typedef unsigned long long 	u64;
typedef	signed long long 	s64;
typedef unsigned int 		u32;
typedef	signed int 			s32;
typedef unsigned short 		u16;
typedef	signed short 		s16;
typedef unsigned char 		u8;
typedef	signed char 		s8;

// Helper
#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0b10000000 ? '1' : '0'), \
  (byte & 0b01000000 ? '1' : '0'), \
  (byte & 0b00100000 ? '1' : '0'), \
  (byte & 0b00010000 ? '1' : '0'), \
  (byte & 0b00001000 ? '1' : '0'), \
  (byte & 0b00000100 ? '1' : '0'), \
  (byte & 0b00000010 ? '1' : '0'), \
  (byte & 0b00000001 ? '1' : '0')

// Array modification
#define MAX_LOAD_EVENTS 20 // eventListener

// Mnemonics
#define RAND_SEED			mod::tools::randomSeed
#define gameInfo 			tp::d_com_inf_game::dComIfG_gameInfo
#define getPlayerPos 		tp::d_map_path_dmap::getMapPlayerPos
#define sysConsolePtr 		tp::jfw_system::systemConsole
#define isLoading			tp::f_op_scene_req::isUsingOfOverlap

// Stage translations for mode::stage::allStages[]
#define Stage_Lakebed_Temple 0
#define Stage_Morpheel 1
#define Stage_Deku_Toad 2
#define Stage_Goron_Mines 3
#define Stage_Fyrus 4
#define Stage_Dangoro 5
#define Stage_Forest_Temple 6
#define Stage_Diababa 7
#define Stage_Ook 8
#define Stage_Temple_of_Time 9
#define Stage_Armogohma 10
#define Stage_Darknut 11
#define Stage_City_in_the_Sky 12
#define Stage_Argorok 13
#define Stage_Aeralfos 14
#define Stage_Palace_of_Twilight 15
#define Stage_Zant_Main 16
#define Stage_Phantom_Zant_1 17
#define Stage_Phantom_Zant_2 18
#define Stage_Zant_Fight 19
#define Stage_Hyrule_Castle 20
#define Stage_Ganondorf_Castle 21
#define Stage_Ganondorf_Field 22
#define Stage_Ganondorf_Defeated 23
#define Stage_Arbiters_Grounds 24
#define Stage_Stallord 25
#define Stage_Death_Sword 26
#define Stage_Snowpeak_Ruins 27
#define Stage_Blizzeta 28
#define Stage_Darkhammer 29
#define Stage_Lanayru_Ice_Puzzle_Cave 30
#define Stage_Cave_of_Ordeals 31
#define Stage_Eldin_Long_Cave 32
#define Stage_Lake_Hylia_Long_Cave 33
#define Stage_Eldin_Goron_Stockcave 34
#define Stage_Grotto_1 35
#define Stage_Grotto_2 36
#define Stage_Grotto_3 37
#define Stage_Grotto_4 38
#define Stage_Grotto_5 39
#define Stage_Faron_Woods_Cave 40
#define Stage_Ordon_Ranch 41
#define Stage_Title_Screen 42
#define Stage_Ordon_Village 43
#define Stage_Ordon_Spring 44
#define Stage_Faron_Woods 45
#define Stage_Kakariko_Village 46
#define Stage_Death_Mountain 47
#define Stage_Kakariko_Graveyard 48
#define Stage_Zoras_River 49
#define Stage_Zoras_Domain 50
#define Stage_Snowpeak 51
#define Stage_Lake_Hylia 52
#define Stage_Castle_Town 53
#define Stage_Sacred_Grove 54
#define Stage_Bublin_Camp 55
#define Stage_Hyrule_Field 56
#define Stage_Outside_Castle_Town 57
#define Stage_Bublin_2 58
#define Stage_Gerudo_Desert 59
#define Stage_Mirror_Chamber 60
#define Stage_Upper_Zoras_River 61
#define Stage_Fishing_Pond 62
#define Stage_Hidden_Village 63
#define Stage_Hidden_Skill 64
#define Stage_Ordon_Interiors 65
#define Stage_Hyrule_Castle_Sewers 66
#define Stage_Coros_Lantern_Shop 67
#define Stage_Kakariko_Interiors 68
#define Stage_Death_Mountain_Sumo_Hall 69
#define Stage_Castle_Town_Interiors 70
#define Stage_Henas_Cabin 71
#define Stage_Impaz_House 72
#define Stage_Castle_Town_Shops 73
#define Stage_Star_Game 74
#define Stage_Sanctuary_Basement 75