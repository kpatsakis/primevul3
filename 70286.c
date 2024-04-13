static void tns_data(ic_stream *ics, tns_info *tns, bitfile *ld)
{
    uint8_t w, filt, i, start_coef_bits, coef_bits;
    uint8_t n_filt_bits = 2;
    uint8_t length_bits = 6;
    uint8_t order_bits = 5;

    if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
    {
        n_filt_bits = 1;
        length_bits = 4;
        order_bits = 3;
    }

    for (w = 0; w < ics->num_windows; w++)
    {
        tns->n_filt[w] = (uint8_t)faad_getbits(ld, n_filt_bits
            DEBUGVAR(1,74,"tns_data(): n_filt"));
#if 0
        printf("%d\n", tns->n_filt[w]);
#endif

        if (tns->n_filt[w])
        {
            if ((tns->coef_res[w] = faad_get1bit(ld
                DEBUGVAR(1,75,"tns_data(): coef_res"))) & 1)
            {
                start_coef_bits = 4;
            } else {
                start_coef_bits = 3;
            }
#if 0
            printf("%d\n", tns->coef_res[w]);
#endif
        }

        for (filt = 0; filt < tns->n_filt[w]; filt++)
        {
            tns->length[w][filt] = (uint8_t)faad_getbits(ld, length_bits
                DEBUGVAR(1,76,"tns_data(): length"));
#if 0
            printf("%d\n", tns->length[w][filt]);
#endif
            tns->order[w][filt]  = (uint8_t)faad_getbits(ld, order_bits
                DEBUGVAR(1,77,"tns_data(): order"));
#if 0
            printf("%d\n", tns->order[w][filt]);
#endif
            if (tns->order[w][filt])
            {
                tns->direction[w][filt] = faad_get1bit(ld
                    DEBUGVAR(1,78,"tns_data(): direction"));
#if 0
                printf("%d\n", tns->direction[w][filt]);
#endif
                tns->coef_compress[w][filt] = faad_get1bit(ld
                    DEBUGVAR(1,79,"tns_data(): coef_compress"));
#if 0
                printf("%d\n", tns->coef_compress[w][filt]);
#endif

                coef_bits = start_coef_bits - tns->coef_compress[w][filt];
                for (i = 0; i < tns->order[w][filt]; i++)
                {
                    tns->coef[w][filt][i] = (uint8_t)faad_getbits(ld, coef_bits
                        DEBUGVAR(1,80,"tns_data(): coef"));
#if 0
                    printf("%d\n", tns->coef[w][filt][i]);
#endif
                }
            }
        }
    }
}
