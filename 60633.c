static int seek_eof_information (WavpackContext *wpc, int64_t *final_index, int get_wrapper)
{
    int64_t restore_pos, last_pos = -1;
    WavpackStreamReader64 *reader = wpc->reader;
    int alt_types = wpc->open_flags & OPEN_ALT_TYPES;
    uint32_t blocks = 0, audio_blocks = 0;
    void *id = wpc->wv_in;
    WavpackHeader wphdr;

    restore_pos = reader->get_pos (id);    // we restore file position when done


    if (reader->get_length (id) > 1048576LL)
        reader->set_pos_rel (id, -1048576, SEEK_END);
    else
        reader->set_pos_abs (id, 0);


    while (1) {
        uint32_t bcount = read_next_header (reader, id, &wphdr);
        int64_t current_pos = reader->get_pos (id);


        if (current_pos == last_pos) {
            reader->set_pos_abs (id, restore_pos);
            return FALSE;
        }

        last_pos = current_pos;


        if (bcount == (uint32_t) -1) {


            if (!blocks) {
                if (current_pos > 2000000LL)
                    reader->set_pos_rel (id, -2000000, SEEK_CUR);
                else
                    reader->set_pos_abs (id, 0);

                continue;
            }


            reader->set_pos_abs (id, restore_pos);
            return TRUE;
        }

        blocks++;


        if (wphdr.block_samples) {
            if (final_index)
                *final_index = GET_BLOCK_INDEX (wphdr) + wphdr.block_samples;

            audio_blocks++;
        }
        else if (!audio_blocks) {
            if (current_pos > 1048576LL)
                reader->set_pos_rel (id, -1048576, SEEK_CUR);
            else
                reader->set_pos_abs (id, 0);

            continue;
        }


        bcount = wphdr.ckSize - sizeof (WavpackHeader) + 8;

        while (bcount >= 2) {
            unsigned char meta_id, c1, c2;
            uint32_t meta_bc, meta_size;

            if (reader->read_bytes (id, &meta_id, 1) != 1 ||
                reader->read_bytes (id, &c1, 1) != 1) {
                    reader->set_pos_abs (id, restore_pos);
                    return FALSE;
            }

            meta_bc = c1 << 1;
            bcount -= 2;

            if (meta_id & ID_LARGE) {
                if (bcount < 2 || reader->read_bytes (id, &c1, 1) != 1 ||
                    reader->read_bytes (id, &c2, 1) != 1) {
                        reader->set_pos_abs (id, restore_pos);
                        return FALSE;
                }

                meta_bc += ((uint32_t) c1 << 9) + ((uint32_t) c2 << 17);
                bcount -= 2;
            }

            meta_size = (meta_id & ID_ODD_SIZE) ? meta_bc - 1 : meta_bc;
            meta_id &= ID_UNIQUE;

            if (get_wrapper && (meta_id == ID_RIFF_TRAILER || (alt_types && meta_id == ID_ALT_TRAILER)) && meta_bc) {
                wpc->wrapper_data = realloc (wpc->wrapper_data, wpc->wrapper_bytes + meta_bc);

                if (!wpc->wrapper_data) {
                    reader->set_pos_abs (id, restore_pos);
                    return FALSE;
                }

                if (reader->read_bytes (id, wpc->wrapper_data + wpc->wrapper_bytes, meta_bc) == meta_bc)
                    wpc->wrapper_bytes += meta_size;
                else {
                    reader->set_pos_abs (id, restore_pos);
                    return FALSE;
                }
            }
            else if (meta_id == ID_MD5_CHECKSUM || (alt_types && meta_id == ID_ALT_MD5_CHECKSUM)) {
                if (meta_bc == 16 && bcount >= 16) {
                    if (reader->read_bytes (id, wpc->config.md5_checksum, 16) == 16)
                        wpc->config.md5_read = TRUE;
                    else {
                        reader->set_pos_abs (id, restore_pos);
                        return FALSE;
                    }
                }
                else
                    reader->set_pos_rel (id, meta_bc, SEEK_CUR);
            }
            else
                reader->set_pos_rel (id, meta_bc, SEEK_CUR);

            bcount -= meta_bc;
        }
    }
}
