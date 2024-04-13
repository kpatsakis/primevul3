static void gain_control_data(bitfile *ld, ic_stream *ics)
{
    uint8_t bd, wd, ad;
    ssr_info *ssr = &(ics->ssr);

    ssr->max_band = (uint8_t)faad_getbits(ld, 2
        DEBUGVAR(1,1000,"gain_control_data(): max_band"));

    if (ics->window_sequence == ONLY_LONG_SEQUENCE)
    {
        for (bd = 1; bd <= ssr->max_band; bd++)
        {
            for (wd = 0; wd < 1; wd++)
            {
                ssr->adjust_num[bd][wd] = (uint8_t)faad_getbits(ld, 3
                    DEBUGVAR(1,1001,"gain_control_data(): adjust_num"));

                for (ad = 0; ad < ssr->adjust_num[bd][wd]; ad++)
                {
                    ssr->alevcode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 4
                        DEBUGVAR(1,1002,"gain_control_data(): alevcode"));
                    ssr->aloccode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 5
                        DEBUGVAR(1,1003,"gain_control_data(): aloccode"));
                }
            }
        }
    } else if (ics->window_sequence == LONG_START_SEQUENCE) {
        for (bd = 1; bd <= ssr->max_band; bd++)
        {
            for (wd = 0; wd < 2; wd++)
            {
                ssr->adjust_num[bd][wd] = (uint8_t)faad_getbits(ld, 3
                    DEBUGVAR(1,1001,"gain_control_data(): adjust_num"));

                for (ad = 0; ad < ssr->adjust_num[bd][wd]; ad++)
                {
                    ssr->alevcode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 4
                        DEBUGVAR(1,1002,"gain_control_data(): alevcode"));
                    if (wd == 0)
                    {
                        ssr->aloccode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 4
                            DEBUGVAR(1,1003,"gain_control_data(): aloccode"));
                    } else {
                        ssr->aloccode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 2
                            DEBUGVAR(1,1003,"gain_control_data(): aloccode"));
                    }
                }
            }
        }
    } else if (ics->window_sequence == EIGHT_SHORT_SEQUENCE) {
        for (bd = 1; bd <= ssr->max_band; bd++)
        {
            for (wd = 0; wd < 8; wd++)
            {
                ssr->adjust_num[bd][wd] = (uint8_t)faad_getbits(ld, 3
                    DEBUGVAR(1,1001,"gain_control_data(): adjust_num"));

                for (ad = 0; ad < ssr->adjust_num[bd][wd]; ad++)
                {
                    ssr->alevcode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 4
                        DEBUGVAR(1,1002,"gain_control_data(): alevcode"));
                    ssr->aloccode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 2
                        DEBUGVAR(1,1003,"gain_control_data(): aloccode"));
                }
            }
        }
    } else if (ics->window_sequence == LONG_STOP_SEQUENCE) {
        for (bd = 1; bd <= ssr->max_band; bd++)
        {
            for (wd = 0; wd < 2; wd++)
            {
                ssr->adjust_num[bd][wd] = (uint8_t)faad_getbits(ld, 3
                    DEBUGVAR(1,1001,"gain_control_data(): adjust_num"));

                for (ad = 0; ad < ssr->adjust_num[bd][wd]; ad++)
                {
                    ssr->alevcode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 4
                        DEBUGVAR(1,1002,"gain_control_data(): alevcode"));

                    if (wd == 0)
                    {
                        ssr->aloccode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 4
                            DEBUGVAR(1,1003,"gain_control_data(): aloccode"));
                    } else {
                        ssr->aloccode[bd][wd][ad] = (uint8_t)faad_getbits(ld, 5
                            DEBUGVAR(1,1003,"gain_control_data(): aloccode"));
                    }
                }
            }
        }
    }
}
