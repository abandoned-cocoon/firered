#pragma once
#include "types.h"
#include "object.h"

#define poke_ability ability

enum pokemon_data_request {
	req_pokemon_id_or_personality = 0x0,
	req_trainer_id = 0x1,
	req_nick = 0x2,
	req_font_lang = 0x3,
	req_sanity_x4 = 0x4,
	req_sanity_x5 = 0x5,
	req_sanity_x6 = 0x6,
	req_ot_name = 0x7,
	req_marks = 0x8,
	req_checksum = 0x9,
	req_species = 0xB,
	req_held_item = 0xC,
	req_move1 = 0xD,
	req_move2 = 0xE,
	req_move3 = 0xF,
	req_move4 = 0x10,
	req_pp1 = 0x11,
	req_pp2 = 0x12,
	req_pp3 = 0x13,
	req_pp4 = 0x14,
	req_pp_bonuses = 0x15,
	req_coolness = 0x16,
	req_beauty = 0x17,
	req_cuteness = 0x18,
	req_exp_points = 0x19,
	req_hp_ev = 0x1A,
	req_atk_ev = 0x1B,
	req_def_ev = 0x1C,
	req_spd_ev = 0x1D,
	req_spatk_ev = 0x1E,
	req_spdef_ev = 0x1F,
	req_happiness = 0x20,
	req_smartness = 0x21,
	req_pokerus = 0x22,
	req_catch_location = 0x23,
	req_catch_level = 0x24,
	req_game_of_origin = 0x25,
	req_hometown_pokeball_trainergender = 0x26, // or just pokeball?
	req_hp_iv = 0x27,
	req_atk_iv = 0x28,
	req_def_iv = 0x29,
	req_spd_iv = 0x2A,
	req_spatk_iv = 0x2B,
	req_spdef_iv = 0x2C,
	req_is_egg = 0x2D,
	req_ability_bit = 0x2E,
	req_toughness = 0x2F,
	req_sheen = 0x30,
	req_ot_gender = 0x31,
	req_x32 = 0x32,
	req_status_ailment = 0x37,
	req_level = 0x38,
	req_current_hp = 0x39,
	req_max_hp = 0x3A,
	req_atk = 0x3B,
	req_def = 0x3C,
	req_spd = 0x3D,
	req_spatk = 0x3E,
	req_spdef = 0x3F,
	req_pokerus_remaining = 0x40,
	req_species2 = 0x41,
	req_hall_of_fame_ribbon = 0x43,
	req_obedience = 0x50,
	req_atk2 = 0x54,
	req_def2 = 0x55,
	req_spd2 = 0x56,
	req_spatk2 = 0x57,
	req_spdef2 = 0x58,
};

enum pokemon_type {
	ty_normal = 0x0,
	ty_fighting = 0x1,
	ty_flying = 0x2,
	ty_poison = 0x3,
	ty_ground = 0x4,
	ty_rock = 0x5,
	ty_bug = 0x6,
	ty_ghost = 0x7,
	ty_steel = 0x8,
	ty____ = 0x9,
	ty_fire = 0xA,
	ty_water = 0xB,
	ty_grass = 0xC,
	ty_electric = 0xD,
	ty_psychic = 0xE,
	ty_ice = 0xF,
	ty_dragon = 0x10,
	ty_dark = 0x11,
};

enum poke_ability {
	ab_none = 0,
	ab_stench = 1,
	ab_drizzle = 2,
	ab_speed_boost = 3,
	ab_battle_armor = 4,
	ab_sturdy = 5,
	ab_damp = 6,
	ab_limber = 7,
	ab_sand_veil = 8,
	ab_static_ = 9,
	ab_volt_absorb = 10,
	ab_water_absorb = 11,
	ab_oblivious = 12,
	ab_cloud_nine = 13,
	ab_compound_eyes = 14,
	ab_insomnia = 15,
	ab_color_change = 16,
	ab_immunity = 17,
	ab_flash_fire = 18,
	ab_shield_dust = 19,
	ab_own_tempo = 20,
	ab_suction_cups = 21,
	ab_intimidate = 22,
	ab_shadow_tag = 23,
	ab_rough_skin = 24,
	ab_wonder_guard = 25,
	ab_levitate = 26,
	ab_effect_spore = 27,
	ab_synchronize = 28,
	ab_clear_body = 29,
	ab_natural_cure = 30,
	ab_lightningrod = 31,
	ab_serene_grace = 32,
	ab_swift_swim = 33,
	ab_chlorophyll = 34,
	ab_illuminate = 35,
	ab_trace = 36,
	ab_huge_power = 37,
	ab_poison_point = 38,
	ab_inner_focus = 39,
	ab_magma_armor = 40,
	ab_water_veil = 41,
	ab_magnet_pull = 42,
	ab_soundproof = 43,
	ab_rain_dish = 44,
	ab_sand_stream = 45,
	ab_pressure = 46,
	ab_thick_fat = 47,
	ab_early_bird = 48,
	ab_flame_body = 49,
	ab_run_away = 50,
	ab_keen_eye = 51,
	ab_hyper_cutter = 52,
	ab_pickup = 53,
	ab_truant = 54,
	ab_hustle = 55,
	ab_cute_charm = 56,
	ab_plus = 57,
	ab_minus = 58,
	ab_forecast = 59,
	ab_sticky_hold = 60,
	ab_shed_skin = 61,
	ab_guts = 62,
	ab_marvel_scale = 63,
	ab_liquid_ooze = 64,
	ab_overgrow = 65,
	ab_blaze = 66,
	ab_torrent = 67,
	ab_swarm = 68,
	ab_rock_head = 69,
	ab_drought = 70,
	ab_arena_trap = 71,
	ab_vital_spirit = 72,
	ab_white_smoke = 73,
	ab_pure_power = 74,
	ab_shell_armor = 75,
	ab_cacophony = 76,
	ab_air_lock = 77,
	ab_tangled_feet = 78,
	ab_motor_drive = 79,
	ab_rivalry = 80,
	ab_steadyfast = 81,
	ab_snow_cloak = 82,
	ab_gluttony = 83,
	ab_anger_point = 84,
	ab_unburden = 85,
	ab_heatproof = 86,
	ab_simple = 87,
	ab_dry_skin = 88,
	ab_download = 89,
	ab_iron_fist = 90,
	ab_poison_heal = 91,
	ab_adaptability = 92,
	ab_skill_link = 93,
	ab_hydration = 94,
	ab_solar_power = 95,
	ab_quick_feet = 96,
	ab_normalize = 97,
	ab_sniper = 98,
	ab_magic_guard = 99,
	ab_no_guard = 100,
	ab_stall = 101,
	ab_technician = 102,
	ab_leaf_guard = 103,
	ab_klutz = 104,
	ab_mold_breaker = 105,
	ab_super_luck = 106,
	ab_aftermath = 107,
	ab_anticipation = 108,
	ab_forewarn = 109,
	ab_unaware = 110,
	ab_tinted_lens = 111,
	ab_filter = 112,
	ab_slow_start = 113,
	ab_scrappy = 114,
	ab_storm_drain = 115,
	ab_ice_body = 116,
	ab_solid_rock = 117,
	ab_snow_warning = 118,
	ab_honey_gather = 119,
	ab_frisk = 120,
	ab_reckless = 121,
	ab_multitype = 122,
	ab_flower_gift = 123,
	ab_bad_dreams = 124,
	ab_pickpocket = 125,
	ab_sheer_force = 126,
	ab_contrary = 127,
	ab_unnerve = 128,
	ab_defiant = 129,
};

enum poke_nature {
	NATURE_HARDY = 0x0,
	NATURE_LONELY = 0x1,
	NATURE_BRAVE = 0x2,
	NATURE_ADAMANT = 0x3,
	NATURE_NAUGHTY = 0x4,
	NATURE_BOLD = 0x5,
	NATURE_DOCILE = 0x6,
	NATURE_RELAXED = 0x7,
	NATURE_IMPISH = 0x8,
	NATURE_LAX = 0x9,
	NATURE_TIMID = 0xA,
	NATURE_HASTY = 0xB,
	NATURE_SERIOUS = 0xC,
	NATURE_JOLLY = 0xD,
	NATURE_NAIVE = 0xE,
	NATURE_MODEST = 0xF,
	NATURE_MILD = 0x10,
	NATURE_QUIET = 0x11,
	NATURE_BASHFUL = 0x12,
	NATURE_RASH = 0x13,
	NATURE_CALM = 0x14,
	NATURE_GENTLE = 0x15,
	NATURE_SASSY = 0x16,
	NATURE_CAREFUL = 0x17,
	NATURE_QUIRKY = 0x18,
};

enum poke_indices {
	poke_missingno = 0x0,
	poke_bulbasaur = 0x1,
	poke_ivysaur = 0x2,
	poke_venusaur = 0x3,
	poke_charmander = 0x4,
	poke_charmeleon = 0x5,
	poke_charizard = 0x6,
	poke_squirtle = 0x7,
	poke_wartortle = 0x8,
	poke_blastoise = 0x9,
	poke_caterpie = 0xA,
	poke_metapod = 0xB,
	poke_butterfree = 0xC,
	poke_weedle = 0xD,
	poke_kakuna = 0xE,
	poke_beedrill = 0xF,
	poke_pidgey = 0x10,
	poke_pidgeotto = 0x11,
	poke_pidgeot = 0x12,
	poke_rattata = 0x13,
	poke_raticate = 0x14,
	poke_spearow = 0x15,
	poke_fearow = 0x16,
	poke_ekans = 0x17,
	poke_arbok = 0x18,
	poke_pikachu = 0x19,
	poke_raichu = 0x1A,
	poke_sandshrew = 0x1B,
	poke_sandslash = 0x1C,
	poke_nidoranf = 0x1D,
	poke_nidorina = 0x1E,
	poke_nidoqueen = 0x1F,
	poke_nidoranm = 0x20,
	poke_nidorino = 0x21,
	poke_nidoking = 0x22,
	poke_clefairy = 0x23,
	poke_clefable = 0x24,
	poke_vulpix = 0x25,
	poke_ninetales = 0x26,
	poke_jigglypuff = 0x27,
	poke_wigglytuff = 0x28,
	poke_zubat = 0x29,
	poke_golbat = 0x2A,
	poke_oddish = 0x2B,
	poke_gloom = 0x2C,
	poke_vileplume = 0x2D,
	poke_paras = 0x2E,
	poke_parasect = 0x2F,
	poke_venonat = 0x30,
	poke_venomoth = 0x31,
	poke_diglett = 0x32,
	poke_dugtrio = 0x33,
	poke_meowth = 0x34,
	poke_persian = 0x35,
	poke_psyduck = 0x36,
	poke_golduck = 0x37,
	poke_mankey = 0x38,
	poke_primeape = 0x39,
	poke_growlithe = 0x3A,
	poke_arcanine = 0x3B,
	poke_poliwag = 0x3C,
	poke_poliwhirl = 0x3D,
	poke_poliwrath = 0x3E,
	poke_abra = 0x3F,
	poke_kadabra = 0x40,
	poke_alakazam = 0x41,
	poke_machop = 0x42,
	poke_machoke = 0x43,
	poke_machamp = 0x44,
	poke_bellsprout = 0x45,
	poke_weepinbell = 0x46,
	poke_victreebel = 0x47,
	poke_tentacool = 0x48,
	poke_tentacruel = 0x49,
	poke_geodude = 0x4A,
	poke_graveler = 0x4B,
	poke_golem = 0x4C,
	poke_ponyta = 0x4D,
	poke_rapidash = 0x4E,
	poke_slowpoke = 0x4F,
	poke_slowbro = 0x50,
	poke_magnemite = 0x51,
	poke_magneton = 0x52,
	poke_farfetched = 0x53,
	poke_doduo = 0x54,
	poke_dodrio = 0x55,
	poke_seel = 0x56,
	poke_dewgong = 0x57,
	poke_grimer = 0x58,
	poke_muk = 0x59,
	poke_shellder = 0x5A,
	poke_cloyster = 0x5B,
	poke_gastly = 0x5C,
	poke_haunter = 0x5D,
	poke_gengar = 0x5E,
	poke_onix = 0x5F,
	poke_drowzee = 0x60,
	poke_hypno = 0x61,
	poke_krabby = 0x62,
	poke_kingler = 0x63,
	poke_voltorb = 0x64,
	poke_electrode = 0x65,
	poke_exeggcute = 0x66,
	poke_exeggutor = 0x67,
	poke_cubone = 0x68,
	poke_marowak = 0x69,
	poke_hitmonlee = 0x6A,
	poke_hitmonchan = 0x6B,
	poke_lickitung = 0x6C,
	poke_koffing = 0x6D,
	poke_weezing = 0x6E,
	poke_rhyhorn = 0x6F,
	poke_rhydon = 0x70,
	poke_chansey = 0x71,
	poke_tangela = 0x72,
	poke_kangaskhan = 0x73,
	poke_horsea = 0x74,
	poke_seadra = 0x75,
	poke_goldeen = 0x76,
	poke_seaking = 0x77,
	poke_staryu = 0x78,
	poke_starmie = 0x79,
	poke_mrmime = 0x7A,
	poke_scyther = 0x7B,
	poke_jynx = 0x7C,
	poke_electabuzz = 0x7D,
	poke_magmar = 0x7E,
	poke_pinsir = 0x7F,
	poke_tauros = 0x80,
	poke_magikarp = 0x81,
	poke_gyarados = 0x82,
	poke_lapras = 0x83,
	poke_ditto = 0x84,
	poke_eevee = 0x85,
	poke_vaporeon = 0x86,
	poke_jolteon = 0x87,
	poke_flareon = 0x88,
	poke_porygon = 0x89,
	poke_omanyte = 0x8A,
	poke_omastar = 0x8B,
	poke_kabuto = 0x8C,
	poke_kabutops = 0x8D,
	poke_aerodactyl = 0x8E,
	poke_snorlax = 0x8F,
	poke_articuno = 0x90,
	poke_zapdos = 0x91,
	poke_moltres = 0x92,
	poke_dratini = 0x93,
	poke_dragonair = 0x94,
	poke_dragonite = 0x95,
	poke_mewtwo = 0x96,
	poke_mew = 0x97,
	poke_chikorita = 0x98,
	poke_bayleef = 0x99,
	poke_meganium = 0x9A,
	poke_cyndaquil = 0x9B,
	poke_quilava = 0x9C,
	poke_typhlosion = 0x9D,
	poke_totodile = 0x9E,
	poke_croconaw = 0x9F,
	poke_feraligatr = 0xA0,
	poke_sentret = 0xA1,
	poke_furret = 0xA2,
	poke_hoothoot = 0xA3,
	poke_noctowl = 0xA4,
	poke_ledyba = 0xA5,
	poke_ledian = 0xA6,
	poke_spinarak = 0xA7,
	poke_ariados = 0xA8,
	poke_crobat = 0xA9,
	poke_chinchou = 0xAA,
	poke_lanturn = 0xAB,
	poke_pichu = 0xAC,
	poke_cleffa = 0xAD,
	poke_igglybuff = 0xAE,
	poke_togepi = 0xAF,
	poke_togetic = 0xB0,
	poke_natu = 0xB1,
	poke_xatu = 0xB2,
	poke_mareep = 0xB3,
	poke_flaaffy = 0xB4,
	poke_ampharos = 0xB5,
	poke_bellossom = 0xB6,
	poke_marill = 0xB7,
	poke_azumarill = 0xB8,
	poke_sudowoodo = 0xB9,
	poke_politoed = 0xBA,
	poke_hoppip = 0xBB,
	poke_skiploom = 0xBC,
	poke_jumpluff = 0xBD,
	poke_aipom = 0xBE,
	poke_sunkern = 0xBF,
	poke_sunflora = 0xC0,
	poke_yanma = 0xC1,
	poke_wooper = 0xC2,
	poke_quagsire = 0xC3,
	poke_espeon = 0xC4,
	poke_umbreon = 0xC5,
	poke_murkrow = 0xC6,
	poke_slowking = 0xC7,
	poke_misdreavus = 0xC8,
	poke_unown = 0xC9,
	poke_wobbuffet = 0xCA,
	poke_girafarig = 0xCB,
	poke_pineco = 0xCC,
	poke_forretress = 0xCD,
	poke_dunsparce = 0xCE,
	poke_gligar = 0xCF,
	poke_steelix = 0xD0,
	poke_snubbull = 0xD1,
	poke_granbull = 0xD2,
	poke_qwilfish = 0xD3,
	poke_scizor = 0xD4,
	poke_shuckle = 0xD5,
	poke_heracross = 0xD6,
	poke_sneasel = 0xD7,
	poke_teddiursa = 0xD8,
	poke_ursaring = 0xD9,
	poke_slugma = 0xDA,
	poke_magcargo = 0xDB,
	poke_swinub = 0xDC,
	poke_piloswine = 0xDD,
	poke_corsola = 0xDE,
	poke_remoraid = 0xDF,
	poke_octillery = 0xE0,
	poke_delibird = 0xE1,
	poke_mantine = 0xE2,
	poke_skarmory = 0xE3,
	poke_houndour = 0xE4,
	poke_houndoom = 0xE5,
	poke_kingdra = 0xE6,
	poke_phanpy = 0xE7,
	poke_donphan = 0xE8,
	poke_porygon2 = 0xE9,
	poke_stantler = 0xEA,
	poke_smeargle = 0xEB,
	poke_tyrogue = 0xEC,
	poke_hitmontop = 0xED,
	poke_smoochum = 0xEE,
	poke_elekid = 0xEF,
	poke_magby = 0xF0,
	poke_miltank = 0xF1,
	poke_blissey = 0xF2,
	poke_raikou = 0xF3,
	poke_entei = 0xF4,
	poke_suicune = 0xF5,
	poke_larvitar = 0xF6,
	poke_pupitar = 0xF7,
	poke_tyranitar = 0xF8,
	poke_lugia = 0xF9,
	poke_hooh = 0xFA,
	poke_celebi = 0xFB,
	poke_treecko = 0x115,
	poke_grovyle = 0x116,
	poke_sceptile = 0x117,
	poke_torchic = 0x118,
	poke_combusken = 0x119,
	poke_blaziken = 0x11A,
	poke_mudkip = 0x11B,
	poke_marshtomp = 0x11C,
	poke_swampert = 0x11D,
	poke_poochyena = 0x11E,
	poke_mightyena = 0x11F,
	poke_zigzagoon = 0x120,
	poke_linoone = 0x121,
	poke_wurmple = 0x122,
	poke_silcoon = 0x123,
	poke_beautifly = 0x124,
	poke_cascoon = 0x125,
	poke_dustox = 0x126,
	poke_lotad = 0x127,
	poke_lombre = 0x128,
	poke_ludicolo = 0x129,
	poke_seedot = 0x12A,
	poke_nuzleaf = 0x12B,
	poke_shiftry = 0x12C,
	poke_nincada = 0x12D,
	poke_ninjask = 0x12E,
	poke_shedinja = 0x12F,
	poke_taillow = 0x130,
	poke_swellow = 0x131,
	poke_shroomish = 0x132,
	poke_breloom = 0x133,
	poke_spinda = 0x134,
	poke_wingull = 0x135,
	poke_pelipper = 0x136,
	poke_surskit = 0x137,
	poke_masquerain = 0x138,
	poke_wailmer = 0x139,
	poke_wailord = 0x13A,
	poke_skitty = 0x13B,
	poke_delcatty = 0x13C,
	poke_kecleon = 0x13D,
	poke_baltoy = 0x13E,
	poke_claydol = 0x13F,
	poke_nosepass = 0x140,
	poke_torkoal = 0x141,
	poke_sableye = 0x142,
	poke_barboach = 0x143,
	poke_whiscash = 0x144,
	poke_luvdisc = 0x145,
	poke_corphish = 0x146,
	poke_crawdaunt = 0x147,
	poke_feebas = 0x148,
	poke_milotic = 0x149,
	poke_carvanha = 0x14A,
	poke_sharpedo = 0x14B,
	poke_trapinch = 0x14C,
	poke_vibrava = 0x14D,
	poke_flygon = 0x14E,
	poke_makuhita = 0x14F,
	poke_hariyama = 0x150,
	poke_electrike = 0x151,
	poke_manectric = 0x152,
	poke_numel = 0x153,
	poke_camerupt = 0x154,
	poke_spheal = 0x155,
	poke_sealeo = 0x156,
	poke_walrein = 0x157,
	poke_cacnea = 0x158,
	poke_cacturne = 0x159,
	poke_snorunt = 0x15A,
	poke_glalie = 0x15B,
	poke_lunatone = 0x15C,
	poke_solrock = 0x15D,
	poke_azurill = 0x15E,
	poke_spoink = 0x15F,
	poke_grumpig = 0x160,
	poke_plusle = 0x161,
	poke_minun = 0x162,
	poke_mawile = 0x163,
	poke_meditite = 0x164,
	poke_medicham = 0x165,
	poke_swablu = 0x166,
	poke_altaria = 0x167,
	poke_wynaut = 0x168,
	poke_duskull = 0x169,
	poke_dusclops = 0x16A,
	poke_roselia = 0x16B,
	poke_slakoth = 0x16C,
	poke_vigoroth = 0x16D,
	poke_slaking = 0x16E,
	poke_gulpin = 0x16F,
	poke_swalot = 0x170,
	poke_tropius = 0x171,
	poke_whismur = 0x172,
	poke_loudred = 0x173,
	poke_exploud = 0x174,
	poke_clamperl = 0x175,
	poke_huntail = 0x176,
	poke_gorebyss = 0x177,
	poke_absol = 0x178,
	poke_shuppet = 0x179,
	poke_banette = 0x17A,
	poke_seviper = 0x17B,
	poke_zangoose = 0x17C,
	poke_relicanth = 0x17D,
	poke_aron = 0x17E,
	poke_lairon = 0x17F,
	poke_aggron = 0x180,
	poke_castform = 0x181,
	poke_volbeat = 0x182,
	poke_illumise = 0x183,
	poke_lileep = 0x184,
	poke_cradily = 0x185,
	poke_anorith = 0x186,
	poke_armaldo = 0x187,
	poke_ralts = 0x188,
	poke_kirlia = 0x189,
	poke_gardevoir = 0x18A,
	poke_bagon = 0x18B,
	poke_shelgon = 0x18C,
	poke_salamence = 0x18D,
	poke_beldum = 0x18E,
	poke_metang = 0x18F,
	poke_metagross = 0x190,
	poke_regirock = 0x191,
	poke_regice = 0x192,
	poke_registeel = 0x193,
	poke_kyogre = 0x194,
	poke_groudon = 0x195,
	poke_rayquaza = 0x196,
	poke_latias = 0x197,
	poke_latios = 0x198,
	poke_jirachi = 0x199,
	poke_deoxys = 0x19A,
	poke_chimecho = 0x19B,
	species_max = 0x19C,
};

struct pokemon_boxed {
	u8  personality;
	u32 otid;
	u8  nick[10];
	u8  font;
	u8  sanity;
	u8  otname[7];
	u8  mark;
	u16 checksum;
	u16 field_1E;
	u8  substruct[48];
};

struct pokemon_sub_attacks {
	u16 move[4];
	u8 pp[4];
};

struct pokemon_sub_condition {
	u8 hp_ev;
	u8 atk_ev;
	u8 def_ev;
	u8 spd_ev;
	u8 spatk_ev;
	u8 spdef_ev;
	u8 coolness;
	u8 beauty;
	u8 cuteness;
	u8 smartness;
	u8 toughness;
	u8 feel;
};

struct pokemon_sub_growth {
	u16 species;
	u16 held_item;
	u32 exp;
	u8 pp_bonuses;
	u8 friendship;
	u16 unknown;
};

struct pokemon_sub_misc {
	u8 pokerus;
	u8 met_location;
	u16 origins_info;
	u32 ivs_egg_ability;
	u32 ribbons_obedience;
};

struct pokemon_extended {
	struct pokemon_boxed boxed;
	u32 ailment;
	u8  level;
	u8  field_55;
	u16 current_hp;
	u16 total_hp;
	u16 atk;
	u16 def;
	u16 spd;
	u16 spatk;
	u16 spdef;
};

struct pokemon_base {
	u8 hp;
	u8 atk;
	u8 def;
	u8 spd;
	u8 spatk;
	u8 spdef;
	u8 type1;
	u8 type2;
	u8 catch_rate;
	u8 exp_yield;
	u16 ev_yield;
	u16 item1;
	u16 item2;
	u8 gender_ratio;
	u8 steps_to_hatch;
	u8 base_friendship;
	u8 exp_curve;
	u8 egg_group1;
	u8 egg_group2;
	u8 ability_1;
	u8 ability_2;
	u8 safari_zone_flee_rate;
	u8 color;
	u16 padding;
};

// 02024029
extern u8 party_player_count; // 02024029, poke_quantity
extern u8 byte_0202402A; // 0202402A

extern struct pokemon_extended party_opponent[6]; // 0202402C
extern struct pokemon_extended party_player[6]; // 02024284
extern struct proto_t objt_pokemon[5];

extern u16 parent_a_moveset[4]; // 0202455C
extern u16 child_moveset[4]; // 02024564
extern u16 eggmove_buffer[10]; // 0202456C
extern u16 parent_b_moveset[4]; // 02024580
// continue in save.h

extern struct pokemon_base base_stats[]; // 08254784 // TODO: Rename

extern u8 *pokemon_names[];
