static GF_ESD *FFD_GetESDescriptor(FFDemux *ffd, Bool for_audio)
{
	GF_BitStream *bs;
	Bool dont_use_sl;
	GF_ESD *esd = (GF_ESD *) gf_odf_desc_esd_new(0);
	esd->ESID = 1 + (for_audio ? ffd->audio_st : ffd->video_st);
	esd->decoderConfig->streamType = for_audio ? GF_STREAM_AUDIO : GF_STREAM_VISUAL;
	esd->decoderConfig->avgBitrate = esd->decoderConfig->maxBitrate = 0;

	/*remap std object types - depending on input formats, FFMPEG may not have separate DSI from initial frame.
	In this case we have no choice but using FFMPEG decoders*/
	if (for_audio) {
		AVCodecContext *dec = ffd->ctx->streams[ffd->audio_st]->codec;
		esd->slConfig->timestampResolution = ffd->audio_tscale.den;
		switch (dec->codec_id) {
		case CODEC_ID_MP2:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_AUDIO_MPEG1;
			break;
		case CODEC_ID_MP3:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_AUDIO_MPEG2_PART3;
			break;
		case CODEC_ID_AAC:
			if (!dec->extradata_size) goto opaque_audio;
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_AUDIO_AAC_MPEG4;
			esd->decoderConfig->decoderSpecificInfo->dataLength = dec->extradata_size;
			esd->decoderConfig->decoderSpecificInfo->data = (char*)gf_malloc(sizeof(char)*dec->extradata_size);
			memcpy(esd->decoderConfig->decoderSpecificInfo->data,
			       dec->extradata,
			       sizeof(char)*dec->extradata_size);
			break;
		default:
opaque_audio:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_MEDIA_FFMPEG;
			bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
			gf_bs_write_u32(bs, dec->codec_id);
			gf_bs_write_u32(bs, dec->sample_rate);
			gf_bs_write_u16(bs, dec->channels);
			gf_bs_write_u16(bs, dec->frame_size);
			gf_bs_write_u8(bs, 16);
			gf_bs_write_u8(bs, 0);
			/*ffmpeg specific*/
			gf_bs_write_u16(bs, dec->block_align);
			gf_bs_write_u32(bs, dec->bit_rate);
			gf_bs_write_u32(bs, dec->codec_tag);
			if (dec->extradata_size) {
				gf_bs_write_data(bs, (char *) dec->extradata, dec->extradata_size);
			}
			gf_bs_get_content(bs, (char **) &esd->decoderConfig->decoderSpecificInfo->data, &esd->decoderConfig->decoderSpecificInfo->dataLength);
			gf_bs_del(bs);
			break;
		}
		dont_use_sl = ffd->unreliable_audio_timing;
	} else {
		AVCodecContext *dec = ffd->ctx->streams[ffd->video_st]->codec;
		esd->slConfig->timestampResolution = ffd->video_tscale.den;
		switch (dec->codec_id) {
		case CODEC_ID_MPEG4:
			/*there is a bug in fragmentation of raw H264 in ffmpeg, the NALU startcode (0x00000001) is split across
			two frames - we therefore force internal ffmpeg codec ID to avoid NALU size recompute
			at the decoder level*/
			/*if dsi not detected force use ffmpeg*/
			if (!dec->extradata_size) goto opaque_video;
			/*otherwise use any MPEG-4 Visual*/
			esd->decoderConfig->objectTypeIndication = (dec->codec_id==CODEC_ID_H264) ? GPAC_OTI_VIDEO_AVC : GPAC_OTI_VIDEO_MPEG4_PART2;
			esd->decoderConfig->decoderSpecificInfo->dataLength = dec->extradata_size;
			esd->decoderConfig->decoderSpecificInfo->data = (char*)gf_malloc(sizeof(char)*dec->extradata_size);
			memcpy(esd->decoderConfig->decoderSpecificInfo->data,
			       dec->extradata,
			       sizeof(char)*dec->extradata_size);
			break;
		case CODEC_ID_MPEG1VIDEO:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_MPEG1;
			break;
		case CODEC_ID_MPEG2VIDEO:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_VIDEO_MPEG2_MAIN;
			break;

		case CODEC_ID_H263:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_MEDIA_GENERIC;
			bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
			gf_bs_write_u32(bs, GF_4CC('s', '2', '6', '3') );
			gf_bs_write_u16(bs, dec->width);
			gf_bs_write_u16(bs, dec->height);
			gf_bs_get_content(bs, (char **) &esd->decoderConfig->decoderSpecificInfo->data, &esd->decoderConfig->decoderSpecificInfo->dataLength);
			gf_bs_del(bs);
			break;
		default:
opaque_video:
			esd->decoderConfig->objectTypeIndication = GPAC_OTI_MEDIA_FFMPEG;
			bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
			gf_bs_write_u32(bs, dec->codec_id);
			gf_bs_write_u16(bs, dec->width);
			gf_bs_write_u16(bs, dec->height);
			/*ffmpeg specific*/
			gf_bs_write_u32(bs, dec->bit_rate);
			gf_bs_write_u32(bs, dec->codec_tag);
			gf_bs_write_u32(bs, dec->pix_fmt);

			if (dec->extradata_size) {
				gf_bs_write_data(bs, (char *) dec->extradata, dec->extradata_size);
			}
			gf_bs_get_content(bs, (char **) &esd->decoderConfig->decoderSpecificInfo->data, &esd->decoderConfig->decoderSpecificInfo->dataLength);
			gf_bs_del(bs);
			break;
		}
		dont_use_sl = GF_FALSE;
	}

	if (dont_use_sl) {
		esd->slConfig->predefined = SLPredef_SkipSL;
	} else {
		/*only send full AUs*/
		esd->slConfig->useAccessUnitStartFlag = esd->slConfig->useAccessUnitEndFlag = 0;
		if (for_audio) {
			esd->slConfig->hasRandomAccessUnitsOnlyFlag = 1;
		} else {
			esd->slConfig->useRandomAccessPointFlag = 1;
		}
		esd->slConfig->useTimestampsFlag = 1;
	}

	return esd;
}
