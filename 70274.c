static uint32_t latmAudioMuxElement(latm_header *latm, bitfile *ld)
{
    uint32_t ascLen, asc_bits=0;
    uint32_t x1, y1, m, n, i;
    program_config pce;
    mp4AudioSpecificConfig mp4ASC;

    latm->useSameStreamMux = (uint8_t)faad_getbits(ld, 1);
    if(!latm->useSameStreamMux)
    {
        latm->version = (uint8_t) faad_getbits(ld, 1);
        if(latm->version)
            latm->versionA = (uint8_t) faad_getbits(ld, 1);
        if(latm->versionA)
        {
            fprintf(stderr, "versionA not supported\n");
            return 0;
        }
        if(latm->version) //read taraBufferFullness
            latm_get_value(ld);
        latm->allStreamsSameTimeFraming = (uint8_t)faad_getbits(ld, 1);
        latm->numSubFrames = (uint8_t)faad_getbits(ld, 6) + 1;
        latm->numPrograms = (uint8_t)faad_getbits(ld, 4) + 1;
        latm->numLayers = faad_getbits(ld, 3) + 1;
        if(latm->numPrograms>1 || !latm->allStreamsSameTimeFraming || latm->numSubFrames>1 || latm->numLayers>1)
        {
            fprintf(stderr, "\r\nUnsupported LATM configuration: %d programs/ %d subframes, %d layers, allstreams: %d\n",
                latm->numPrograms, latm->numSubFrames, latm->numLayers, latm->allStreamsSameTimeFraming);
            return 0;
        }
        ascLen = 0;
        if(latm->version)
            ascLen = latm_get_value(ld);

        x1 = faad_get_processed_bits(ld);
        if(AudioSpecificConfigFromBitfile(ld, &mp4ASC, &pce, 0, 1) < 0)
            return 0;

        y1 = faad_get_processed_bits(ld);
        if((y1-x1) <= MAX_ASC_BYTES*8)
        {
            faad_rewindbits(ld);
            m = x1;
            while(m>0)
            {
                n = min(m, 32);
                faad_getbits(ld, n);
                m -= n;
            }

            i = 0;
            m = latm->ASCbits = y1 - x1;
            while(m > 0)
            {
                n = min(m, 8);
                latm->ASC[i++] = (uint8_t) faad_getbits(ld, n);
                m -= n;
            }
        }

        asc_bits = y1-x1;

        if(ascLen>asc_bits)
            faad_getbits(ld, ascLen-asc_bits);

        latm->framelen_type = (uint8_t) faad_getbits(ld, 3);
        if(latm->framelen_type == 0)
        {
            latm->frameLength = 0;
            faad_getbits(ld, 8); //buffer fullness for frame_len_type==0, useless
        }
        else if(latm->framelen_type == 1)
        {
            latm->frameLength = faad_getbits(ld, 9);
            if(latm->frameLength==0)
            {
                fprintf(stderr, "Invalid frameLength: 0\r\n");
                return 0;
            }
            latm->frameLength = (latm->frameLength+20)*8;
        }
        else
        {   //hellish CELP or HCVX stuff, discard
            fprintf(stderr, "Unsupported CELP/HCVX framelentype: %d\n", latm->framelen_type);
            return 0;
        }

        latm->otherDataLenBits = 0;
        if(faad_getbits(ld, 1))
        {   //other data present
            int esc, tmp;
            if(latm->version)
                latm->otherDataLenBits = latm_get_value(ld);
            else do
            {
                esc = faad_getbits(ld, 1);
                tmp = faad_getbits(ld, 8);
                latm->otherDataLenBits = (latm->otherDataLenBits << 8) + tmp;
            } while(esc);
        }
        if(faad_getbits(ld, 1)) //crc
            faad_getbits(ld, 8);
        latm->inited = 1;
      }

      if(latm->inited)
          return latmParsePayload(latm, ld);
      else
          return 0;
}
