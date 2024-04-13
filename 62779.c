static void next_start_code_studio(GetBitContext *gb)
{
    align_get_bits(gb);

    while (get_bits_left(gb) >= 24 && show_bits_long(gb, 24) != 0x1) {
        get_bits(gb, 8);
    }
}
