void DRM_aac_scalable_main_element(NeAACDecStruct *hDecoder, NeAACDecFrameInfo *hInfo,
                                   bitfile *ld, program_config *pce, drc_info *drc)
{
    uint8_t retval = 0;
    uint8_t channels = hDecoder->fr_channels = 0;
    uint8_t ch;
    uint8_t this_layer_stereo = (hDecoder->channelConfiguration > 1) ? 1 : 0;
    element cpe = {0};
    ic_stream *ics1 = &(cpe.ics1);
    ic_stream *ics2 = &(cpe.ics2);
    int16_t *spec_data;
    ALIGN int16_t spec_data1[1024] = {0};
    ALIGN int16_t spec_data2[1024] = {0};

    hDecoder->fr_ch_ele = 0;

    hInfo->error = DRM_aac_scalable_main_header(hDecoder, ics1, ics2, ld, this_layer_stereo);
    if (hInfo->error > 0)
        return;

    cpe.common_window = 1;
    if (this_layer_stereo)
    {
        hDecoder->element_id[0] = ID_CPE;
        if (hDecoder->element_output_channels[hDecoder->fr_ch_ele] == 0)
            hDecoder->element_output_channels[hDecoder->fr_ch_ele] = 2;
    } else {
        hDecoder->element_id[0] = ID_SCE;
    }

    if (this_layer_stereo)
    {
        cpe.channel        = 0;
        cpe.paired_channel = 1;
    }


    /* Stereo2 / Mono1 */
    ics1->tns_data_present = faad_get1bit(ld);

#if defined(LTP_DEC)
    ics1->ltp.data_present = faad_get1bit(ld);
#elif defined (DRM)
    if(faad_get1bit(ld)) {
         hInfo->error = 26;
         return;
    }
#else
    faad_get1bit(ld);
#endif    

    hInfo->error = side_info(hDecoder, &cpe, ld, ics1, 1);
    if (hInfo->error > 0)
        return;
    if (this_layer_stereo)
    {
        /* Stereo3 */
        ics2->tns_data_present = faad_get1bit(ld);
#ifdef LTP_DEC
        ics1->ltp.data_present =
#endif
            faad_get1bit(ld);
        hInfo->error = side_info(hDecoder, &cpe, ld, ics2, 1);
        if (hInfo->error > 0)
            return;
    }
    /* Stereo4 / Mono2 */
    if (ics1->tns_data_present)
        tns_data(ics1, &(ics1->tns), ld);
    if (this_layer_stereo)
    {
        /* Stereo5 */
        if (ics2->tns_data_present)
            tns_data(ics2, &(ics2->tns), ld);
    }

#ifdef DRM
    /* CRC check */
    if (hDecoder->object_type == DRM_ER_LC)
    {
        if ((hInfo->error = (uint8_t)faad_check_CRC(ld, (uint16_t)faad_get_processed_bits(ld) - 8)) > 0)
            return;
    }
#endif

    /* Stereo6 / Mono3 */
    /* error resilient spectral data decoding */
    if ((hInfo->error = reordered_spectral_data(hDecoder, ics1, ld, spec_data1)) > 0)
    {
        return;
    }
    if (this_layer_stereo)
    {
        /* Stereo7 */
        /* error resilient spectral data decoding */
        if ((hInfo->error = reordered_spectral_data(hDecoder, ics2, ld, spec_data2)) > 0)
        {
            return;
        }
    }


#ifdef DRM
#ifdef SBR_DEC
    /* In case of DRM we need to read the SBR info before channel reconstruction */
    if ((hDecoder->sbr_present_flag == 1) && (hDecoder->object_type == DRM_ER_LC))
    {
        bitfile ld_sbr = {0};
        uint32_t i;
        uint16_t count = 0;
        uint8_t *revbuffer;
        uint8_t *prevbufstart;
        uint8_t *pbufend;

        /* all forward bitreading should be finished at this point */
        uint32_t bitsconsumed = faad_get_processed_bits(ld);
        uint32_t buffer_size = faad_origbitbuffer_size(ld);
        uint8_t *buffer = (uint8_t*)faad_origbitbuffer(ld);

        if (bitsconsumed + 8 > buffer_size*8)
        {
            hInfo->error = 14;
            return;
        }

        if (!hDecoder->sbr[0])
        {
            hDecoder->sbr[0] = sbrDecodeInit(hDecoder->frameLength, hDecoder->element_id[0],
                2*get_sample_rate(hDecoder->sf_index), 0 /* ds SBR */, 1);
        }

        /* Reverse bit reading of SBR data in DRM audio frame */
        revbuffer = (uint8_t*)faad_malloc(buffer_size*sizeof(uint8_t));
        prevbufstart = revbuffer;
        pbufend = &buffer[buffer_size - 1];
        for (i = 0; i < buffer_size; i++)
            *prevbufstart++ = tabFlipbits[*pbufend--];

        /* Set SBR data */
        /* consider 8 bits from AAC-CRC */
        /* SBR buffer size is original buffer size minus AAC buffer size */
        count = (uint16_t)bit2byte(buffer_size*8 - bitsconsumed);
        faad_initbits(&ld_sbr, revbuffer, count);

        hDecoder->sbr[0]->sample_rate = get_sample_rate(hDecoder->sf_index);
        hDecoder->sbr[0]->sample_rate *= 2;

        faad_getbits(&ld_sbr, 8); /* Skip 8-bit CRC */

        hDecoder->sbr[0]->ret = sbr_extension_data(&ld_sbr, hDecoder->sbr[0], count, hDecoder->postSeekResetFlag);
#if (defined(PS_DEC) || defined(DRM_PS))
        if (hDecoder->sbr[0]->ps_used)
        {
            hDecoder->ps_used[0] = 1;
            hDecoder->ps_used_global = 1;
        }
#endif

        if (ld_sbr.error)
        {
            hDecoder->sbr[0]->ret = 1;
        }

        /* check CRC */
        /* no need to check it if there was already an error */
        if (hDecoder->sbr[0]->ret == 0)
            hDecoder->sbr[0]->ret = (uint8_t)faad_check_CRC(&ld_sbr, (uint16_t)faad_get_processed_bits(&ld_sbr) - 8);

        /* SBR data was corrupted, disable it until the next header */
        if (hDecoder->sbr[0]->ret != 0)
        {
            hDecoder->sbr[0]->header_count = 0;
        }

        faad_endbits(&ld_sbr);

        if (revbuffer)
            faad_free(revbuffer);
    }
#endif
#endif

    if (this_layer_stereo)
    {
        hInfo->error = reconstruct_channel_pair(hDecoder, ics1, ics2, &cpe, spec_data1, spec_data2);
        if (hInfo->error > 0)
            return;
    } else {
        hInfo->error = reconstruct_single_channel(hDecoder, ics1, &cpe, spec_data1);
        if (hInfo->error > 0)
            return;
    }

    /* map output channels position to internal data channels */
    if (hDecoder->element_output_channels[hDecoder->fr_ch_ele] == 2)
    {
        /* this might be faulty when pce_set is true */
        hDecoder->internal_channel[channels] = channels;
        hDecoder->internal_channel[channels+1] = channels+1;
    } else {
        hDecoder->internal_channel[channels] = channels;
    }

    hDecoder->fr_channels += hDecoder->element_output_channels[hDecoder->fr_ch_ele];
    hDecoder->fr_ch_ele++;

    return;
}
