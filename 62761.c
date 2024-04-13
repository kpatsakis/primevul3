 static void extension_and_user_data(MpegEncContext *s, GetBitContext *gb, int id)
{
    uint32_t startcode;
    uint8_t extension_type;

    startcode = show_bits_long(gb, 32);
    if (startcode == USER_DATA_STARTCODE || startcode == EXT_STARTCODE) {

        if ((id == 2 || id == 4) && startcode == EXT_STARTCODE) {
            skip_bits_long(gb, 32);
            extension_type = get_bits(gb, 4);
            if (extension_type == QUANT_MATRIX_EXT_ID)
                read_quant_matrix_ext(s, gb);
        }
    }
}
