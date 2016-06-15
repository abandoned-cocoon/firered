#pragma once
#include "continuegame.h"
#include "npc.h"
#include "pokemon.h"
#include "roaming_pokemon.h"
#include "types.h"
#include "uncategorized.h"

typedef char string9[9];
typedef char string12[12];
typedef char string21[21];
struct message { string12 text; };

struct sav1
{
  struct coords16 camera_position;
  struct warpdata location;
  struct warpdata saved_warp;
  struct warpdata field_14;
  u16 field_1C;
  u8  unknown_1E[6];
  struct warpdata save_warp_2;
  u16 some_music_id;
  u8  weather_probably;
  u8  unknown_2F[1];
  u8  flash_used_on_map;
  u8  unknown_31[1];
  u16 mapdata_idx_plus_1;
  u8  num_party_pokemon;
  u8  unknown_35[3];
  struct pokemon_boxed party_pokemon[6];
  u32 money_maybe;
  u8  pcitems[124];
  u32 bag_probably[42];
  u8  ghu_2_x3B8[120];
  u32 field_430[13];
  u8  ghu_1_x464[232];
  u8  field_54C[172];
  u8  pokemon_flags_1[52];
  u8  field_62C[12];
  u16 field_638;
  u32 field_63A;
  u8  field_63E[98];
  struct npc_state npc_states[16];
  struct rom_npc rom_npcs[64];
  u8  field_EE0[64];
  u32 field_F20;
  u8  field_F24[220];
  u16 var_4000[256];
  u8  secure_data[256];
  struct prev_quest_story questlog[4];
  struct message messages[4];
  struct npc_state more_messages_maybe[16];
  u8  field_2F10[112];
  u32 daycare_data;
  u8  field_2F84[332];
  struct roaming_pokemon_info roaming_pokemon_inf;
  u8  field_30EC[28];
  u16 engima_berry_data[9];
  u8  field_311A[458];
  u8  mevent_related_1[332];
  u32 field_3430;
  u8  mevent_related_2[332];
  u8  field_3580[156];
  u32 ereader_script;
  u8  field_3620[4];
  u8  field_3624[1012];
  u8  pokemon_flags_2[52];
  u8  rivals_name[8];
  u8  field_3A54;
  u8  unknown_3A55[127];
  string21 words[10];
  u8  unknown_3BA6[398];
  u8  field_3D34[14];
  u8  field_3D42[12];
  u8  field_3D4E[153];
  u8  last_field;
};

struct sav2
{
  u8  name[8];
  u8  gender;
  u8  savebits;
  u16 trainerid;
  u16 secretid;
  u16 playtime_hours;
  u8  playtime_minutes;
  u8  playtime_seconds;
  u8  playtime_frames;
  u8  options_button_style;
  u8  options_text_speed_maybe;
  u8  field_15[7];
  u32 first_seen_unown_pid;
  u32 first_seen_spinda_pid;
  u8  field_24;
  u8  unknown_25[3];
  u8  pokemon_flags_caught[52];
  u8  pokemon_flags_seen[52];
  u8  unknown_90[24];
  u32 field_A8;
  u8  gamecode;
  u8  field_AD;
  u8  unknown_AE[2];
  u8  field_B0[1012];
  u8  fourCharacters[4];
  u8  field_4A8[1008];
  u8  mapdata[600];
  u16 field_AF0;
  u16 field_AF2;
  u16 field_AF4;
  u16 field_AF6;
  u8  field_AF8[1064];
  u32 obfuscation_xor_key;
  u8  field_F24[127];
  u8  last_byte_in_sav2;
};

struct sav3 {
  u8  preferred_box;
  u8  unknown_1[3];
  u8  unknown_4[33600];
  string9 box_names[14];
  u8  box_background[14];
};

extern struct sav1 *sav1i;
extern struct sav2 *sav2i;
extern struct sav3 *sav3i;

void save_serialize_pokemon();
void save_deserialize_pokemon();
void save_serialize_npcs();
void save_deserialize_npcs();
void save_serialize_game();
void save_deserialize_game();
void save_game(u8 mode);
