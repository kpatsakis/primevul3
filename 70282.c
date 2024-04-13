static uint8_t section_data(NeAACDecStruct *hDecoder, ic_stream *ics, bitfile *ld)
{
    uint8_t g;
    uint8_t sect_esc_val, sect_bits;

    if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
        sect_bits = 3;
    else
        sect_bits = 5;
    sect_esc_val = (1<<sect_bits) - 1;

#if 0
    printf("\ntotal sfb %d\n", ics->max_sfb);
    printf("   sect    top     cb\n");
#endif

    for (g = 0; g < ics->num_window_groups; g++)
    {
        uint8_t k = 0;
        uint8_t i = 0;

        while (k < ics->max_sfb)
        {
#ifdef ERROR_RESILIENCE
            uint8_t vcb11 = 0;
#endif
            uint8_t sfb;
            uint8_t sect_len_incr;
            uint16_t sect_len = 0;
            uint8_t sect_cb_bits = 4;

            /* if "faad_getbits" detects error and returns "0", "k" is never
               incremented and we cannot leave the while loop */
            if (ld->error != 0)
                return 14;

#ifdef ERROR_RESILIENCE
            if (hDecoder->aacSectionDataResilienceFlag)
                sect_cb_bits = 5;
#endif

            ics->sect_cb[g][i] = (uint8_t)faad_getbits(ld, sect_cb_bits
                DEBUGVAR(1,71,"section_data(): sect_cb"));

            if (ics->sect_cb[g][i] == 12)
                return 32;

#if 0
            printf("%d\n", ics->sect_cb[g][i]);
#endif

#ifndef DRM
            if (ics->sect_cb[g][i] == NOISE_HCB)
                ics->noise_used = 1;
#else
            /* PNS not allowed in DRM */
            if (ics->sect_cb[g][i] == NOISE_HCB)
                return 29;
#endif
            if (ics->sect_cb[g][i] == INTENSITY_HCB2 || ics->sect_cb[g][i] == INTENSITY_HCB)
                ics->is_used = 1;

#ifdef ERROR_RESILIENCE
            if (hDecoder->aacSectionDataResilienceFlag)
            {
                if ((ics->sect_cb[g][i] == 11) ||
                    ((ics->sect_cb[g][i] >= 16) && (ics->sect_cb[g][i] <= 32)))
                {
                    vcb11 = 1;
                }
            }
            if (vcb11)
            {
                sect_len_incr = 1;
            } else {
#endif
                sect_len_incr = (uint8_t)faad_getbits(ld, sect_bits
                    DEBUGVAR(1,72,"section_data(): sect_len_incr"));
#ifdef ERROR_RESILIENCE
            }
#endif
            while ((sect_len_incr == sect_esc_val) /* &&
                (k+sect_len < ics->max_sfb)*/)
            {
                sect_len += sect_len_incr;
                sect_len_incr = (uint8_t)faad_getbits(ld, sect_bits
                    DEBUGVAR(1,72,"section_data(): sect_len_incr"));
            }

            sect_len += sect_len_incr;

            ics->sect_start[g][i] = k;
            ics->sect_end[g][i] = k + sect_len;

#if 0
            printf("%d\n", ics->sect_start[g][i]);
#endif
#if 0
            printf("%d\n", ics->sect_end[g][i]);
#endif

            if (ics->window_sequence == EIGHT_SHORT_SEQUENCE)
            {
                if (k + sect_len > 8*15)
                    return 15;
                if (i >= 8*15)
                    return 15;
            } else {
                if (k + sect_len > MAX_SFB)
                    return 15;
                if (i >= MAX_SFB)
                    return 15;
            }

            for (sfb = k; sfb < k + sect_len; sfb++)
            {
                ics->sfb_cb[g][sfb] = ics->sect_cb[g][i];
#if 0
                printf("%d\n", ics->sfb_cb[g][sfb]);
#endif
            }

#if 0
            printf(" %6d %6d %6d\n",
                i,
                ics->sect_end[g][i],
                ics->sect_cb[g][i]);
#endif

            k += sect_len;
            i++;
        }
        ics->num_sec[g] = i;

        /* the sum of all sect_len_incr elements for a given window
         * group shall equal max_sfb */
        if (k != ics->max_sfb)
        {
            return 32;
        }
#if 0
        printf("%d\n", ics->num_sec[g]);
#endif
    }

#if 0
    printf("\n");
#endif

    return 0;
}
