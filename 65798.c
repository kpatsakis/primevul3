static void gf_isom_check_sample_desc(GF_TrackBox *trak)
{
	GF_BitStream *bs;
	GF_UnknownBox *a;
	u32 i;

	if (!trak->Media || !trak->Media->information) {
		GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] Track with no media box !\n" ));
		return;
	}
	if (!trak->Media->information->sampleTable) {
		GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] Track with no sample table !\n" ));
		trak->Media->information->sampleTable = (GF_SampleTableBox *) gf_isom_box_new(GF_ISOM_BOX_TYPE_STBL);
	}

	if (!trak->Media->information->sampleTable->SampleDescription) {
		GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] Track with no sample description box !\n" ));
		trak->Media->information->sampleTable->SampleDescription = (GF_SampleDescriptionBox *) gf_isom_box_new(GF_ISOM_BOX_TYPE_STSD);
		return;
	}

	i=0;
	while ((a = (GF_UnknownBox*)gf_list_enum(trak->Media->information->sampleTable->SampleDescription->other_boxes, &i))) {
		switch (a->type) {
		case GF_ISOM_BOX_TYPE_MP4S:
		case GF_ISOM_BOX_TYPE_ENCS:
		case GF_ISOM_BOX_TYPE_MP4A:
		case GF_ISOM_BOX_TYPE_ENCA:
		case GF_ISOM_BOX_TYPE_MP4V:
		case GF_ISOM_BOX_TYPE_ENCV:
		case GF_ISOM_BOX_TYPE_RESV:
		case GF_ISOM_SUBTYPE_3GP_AMR:
		case GF_ISOM_SUBTYPE_3GP_AMR_WB:
		case GF_ISOM_SUBTYPE_3GP_EVRC:
		case GF_ISOM_SUBTYPE_3GP_QCELP:
		case GF_ISOM_SUBTYPE_3GP_SMV:
		case GF_ISOM_SUBTYPE_3GP_H263:
		case GF_ISOM_BOX_TYPE_GHNT:
		case GF_ISOM_BOX_TYPE_RTP_STSD:
		case GF_ISOM_BOX_TYPE_SRTP_STSD:
		case GF_ISOM_BOX_TYPE_FDP_STSD:
		case GF_ISOM_BOX_TYPE_RRTP_STSD:
		case GF_ISOM_BOX_TYPE_RTCP_STSD:
		case GF_ISOM_BOX_TYPE_METX:
		case GF_ISOM_BOX_TYPE_METT:
		case GF_ISOM_BOX_TYPE_STXT:
		case GF_ISOM_BOX_TYPE_AVC1:
		case GF_ISOM_BOX_TYPE_AVC2:
		case GF_ISOM_BOX_TYPE_AVC3:
		case GF_ISOM_BOX_TYPE_AVC4:
		case GF_ISOM_BOX_TYPE_SVC1:
		case GF_ISOM_BOX_TYPE_MVC1:
		case GF_ISOM_BOX_TYPE_HVC1:
		case GF_ISOM_BOX_TYPE_HEV1:
		case GF_ISOM_BOX_TYPE_HVC2:
		case GF_ISOM_BOX_TYPE_HEV2:
		case GF_ISOM_BOX_TYPE_HVT1:
		case GF_ISOM_BOX_TYPE_LHV1:
		case GF_ISOM_BOX_TYPE_LHE1:
		case GF_ISOM_BOX_TYPE_TX3G:
		case GF_ISOM_BOX_TYPE_TEXT:
		case GF_ISOM_BOX_TYPE_ENCT:
		case GF_ISOM_BOX_TYPE_DIMS:
		case GF_ISOM_BOX_TYPE_AC3:
		case GF_ISOM_BOX_TYPE_EC3:
		case GF_ISOM_BOX_TYPE_LSR1:
		case GF_ISOM_BOX_TYPE_WVTT:
		case GF_ISOM_BOX_TYPE_STPP:
		case GF_ISOM_BOX_TYPE_SBTT:
		case GF_ISOM_BOX_TYPE_MP3:
		case GF_ISOM_BOX_TYPE_JPEG:
		case GF_ISOM_BOX_TYPE_PNG:
		case GF_ISOM_BOX_TYPE_JP2K:
			continue;
		case GF_ISOM_BOX_TYPE_UNKNOWN:
			break;
		default:
			GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] Unexpected box %s in stsd!\n", gf_4cc_to_str(a->type)));
			continue;
		}
		assert(a->type==GF_ISOM_BOX_TYPE_UNKNOWN);

		if (!a->data || (a->dataSize<8) ) {
			GF_LOG(GF_LOG_WARNING, GF_LOG_CONTAINER, ("[iso file] Sample description %s does not have at least 8 bytes!\n", gf_4cc_to_str(a->original_4cc) ));
			continue;
		}
		else if (a->dataSize > a->size) {
			GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] Sample description %s has wrong data size %d!\n", gf_4cc_to_str(a->original_4cc), a->dataSize));
			continue;
		}

		/*only process visual or audio*/
		switch (trak->Media->handler->handlerType) {
        case GF_ISOM_MEDIA_VISUAL:
		case GF_ISOM_MEDIA_AUXV:
		case GF_ISOM_MEDIA_PICT:
		{
			GF_GenericVisualSampleEntryBox *genv;
			/*remove entry*/
			gf_list_rem(trak->Media->information->sampleTable->SampleDescription->other_boxes, i-1);
			genv = (GF_GenericVisualSampleEntryBox *) gf_isom_box_new(GF_ISOM_BOX_TYPE_GNRV);
			bs = gf_bs_new(a->data, a->dataSize, GF_BITSTREAM_READ);
			genv->size = a->size-8;
			gf_isom_video_sample_entry_read((GF_VisualSampleEntryBox *) genv, bs);

			if (gf_bs_available(bs)) {
				u64 pos = gf_bs_get_position(bs);
				GF_Err e = gf_isom_box_array_read((GF_Box *) genv, bs, gf_isom_box_add_default);
				if (e) {
					gf_bs_seek(bs, pos);
					genv->data_size = (u32) gf_bs_available(bs);
					if (genv->data_size) {
						genv->data = a->data;
						a->data = NULL;
						memmove(genv->data, genv->data + pos, genv->data_size);
					}
				} else {
					genv->data_size = 0;
				}
			}
			gf_bs_del(bs);
			if (!genv->data_size && genv->data) {
				gf_free(genv->data);
				genv->data = NULL;
			}

			genv->size = 0;
			genv->EntryType = a->original_4cc;
			gf_isom_box_del((GF_Box *)a);
			gf_list_insert(trak->Media->information->sampleTable->SampleDescription->other_boxes, genv, i-1);
		}
		break;
		case GF_ISOM_MEDIA_AUDIO:
		{
			GF_GenericAudioSampleEntryBox *gena;
			/*remove entry*/
			gf_list_rem(trak->Media->information->sampleTable->SampleDescription->other_boxes, i-1);
			gena = (GF_GenericAudioSampleEntryBox *) gf_isom_box_new(GF_ISOM_BOX_TYPE_GNRA);
			gena->size = a->size-8;
			bs = gf_bs_new(a->data, a->dataSize, GF_BITSTREAM_READ);
			gf_isom_audio_sample_entry_read((GF_AudioSampleEntryBox *) gena, bs);

			if (gf_bs_available(bs)) {
				u64 pos = gf_bs_get_position(bs);
				GF_Err e = gf_isom_box_array_read((GF_Box *) gena, bs, gf_isom_box_add_default);
				if (e) {
					gf_bs_seek(bs, pos);
					gena->data_size = (u32) gf_bs_available(bs);
					if (gena->data_size) {
						gena->data = a->data;
						a->data = NULL;
						memmove(gena->data, gena->data + pos, gena->data_size);
					}
				} else {
					gena->data_size = 0;
				}
			}
			gf_bs_del(bs);
			if (!gena->data_size && gena->data) {
				gf_free(gena->data);
				gena->data = NULL;
			}
			gena->size = 0;
			gena->EntryType = a->original_4cc;
			gf_isom_box_del((GF_Box *)a);
			gf_list_insert(trak->Media->information->sampleTable->SampleDescription->other_boxes, gena, i-1);
		}
		break;

		default:
		{
			GF_Err e;
			GF_GenericSampleEntryBox *genm;
			/*remove entry*/
			gf_list_rem(trak->Media->information->sampleTable->SampleDescription->other_boxes, i-1);
			genm = (GF_GenericSampleEntryBox *) gf_isom_box_new(GF_ISOM_BOX_TYPE_GNRM);
			genm->size = a->size-8;
			bs = gf_bs_new(a->data, a->dataSize, GF_BITSTREAM_READ);

			e = gf_isom_base_sample_entry_read((GF_SampleEntryBox *)genm, bs);
			if (e) return;

			genm->size -= 8;

			if (gf_bs_available(bs)) {
				u64 pos = gf_bs_get_position(bs);
				GF_Err e = gf_isom_box_array_read((GF_Box *) genm, bs, gf_isom_box_add_default);
				if (e) {
					gf_bs_seek(bs, pos);
					genm->data_size = (u32) gf_bs_available(bs);
					if (genm->data_size) {
						genm->data = a->data;
						a->data = NULL;
						memmove(genm->data, genm->data + pos, genm->data_size);
					}
				} else {
					genm->data_size = 0;
				}
			}
			gf_bs_del(bs);
			if (!genm->data_size && genm->data) {
				gf_free(genm->data);
				genm->data = NULL;
			}
			genm->size = 0;

			genm->EntryType = a->original_4cc;
			gf_isom_box_del((GF_Box *)a);
			gf_list_insert(trak->Media->information->sampleTable->SampleDescription->other_boxes, genm, i-1);
		}
		break;
		}

	}
}
