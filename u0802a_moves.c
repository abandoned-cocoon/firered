// 0802A3F8
void b_feature_update_destiny_bond() {
    int attacker_owner = battle_side_get_owner(b_attacker);
    int defender_owner = battle_side_get_owner(b_defender);

    if (battle_data[b_defender].status2 & B_S2_DESTINY_BOND) {
        if (attacker_owner != defender_owner) {
            if (!(b_features_bitfield & 0x1000000))
                b_features_bitfield |= 0x40u;
        }
    }
}
