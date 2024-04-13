GF_Err gf_isom_nalu_sample_rewrite(GF_MediaBox *mdia, GF_ISOSample *sample, u32 sampleNumber, GF_MPEGVisualSampleEntryBox *entry)
{
	Bool is_hevc = GF_FALSE;
	Bool check_cra_bla = GF_TRUE;
	Bool insert_nalu_delim = GF_TRUE;
	Bool force_sei_inspect = GF_FALSE;
	GF_Err e = GF_OK;
	GF_ISOSample *ref_samp;
	GF_BitStream *src_bs, *ref_bs, *dst_bs, *ps_bs, *sei_suffix_bs;
	u32 nal_size, max_size, nal_unit_size_field, extractor_mode;
	Bool rewrite_ps, rewrite_start_codes, insert_vdrd_code;
	s8 nal_type;
	u32 nal_hdr, sabt_ref, i, track_num;
	u32 temporal_id = 0;
	char *buffer;
	GF_ISOFile *file = mdia->mediaTrack->moov->mov;
	GF_TrackReferenceTypeBox *scal = NULL;

	src_bs = ref_bs = dst_bs = ps_bs = sei_suffix_bs = NULL;
	ref_samp = NULL;
	buffer = NULL;

	Track_FindRef(mdia->mediaTrack, GF_ISOM_REF_SCAL, &scal);

	rewrite_ps = (mdia->mediaTrack->extractor_mode & GF_ISOM_NALU_EXTRACT_INBAND_PS_FLAG) ? GF_TRUE : GF_FALSE;
	rewrite_start_codes = (mdia->mediaTrack->extractor_mode & GF_ISOM_NALU_EXTRACT_ANNEXB_FLAG) ? GF_TRUE : GF_FALSE;
	insert_vdrd_code = (mdia->mediaTrack->extractor_mode & GF_ISOM_NALU_EXTRACT_VDRD_FLAG) ? GF_TRUE : GF_FALSE;
	if (!entry->svc_config && !entry->mvc_config && !entry->lhvc_config) insert_vdrd_code = GF_FALSE;
	extractor_mode = mdia->mediaTrack->extractor_mode&0x0000FFFF;

	if (mdia->mediaTrack->extractor_mode & GF_ISOM_NALU_EXTRACT_TILE_ONLY) {
		insert_nalu_delim = GF_FALSE;
	}

	track_num = 1 + gf_list_find(mdia->mediaTrack->moov->trackList, mdia->mediaTrack);

	if ( (extractor_mode != GF_ISOM_NALU_EXTRACT_INSPECT) && !(mdia->mediaTrack->extractor_mode & GF_ISOM_NALU_EXTRACT_TILE_ONLY) ) {
		u32 ref_track, di;
		if (entry->lhvc_config && !entry->hevc_config && !(mdia->mediaTrack->extractor_mode & GF_ISOM_NALU_EXTRACT_LAYER_ONLY)) {
			GF_ISOSample *base_samp;
			if (gf_isom_get_reference_count(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_SCAL) <= 0) {
				if ( gf_isom_get_reference_count(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_BASE) >= 1) {
					gf_isom_get_reference(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_BASE, 1, &ref_track);
					switch (gf_isom_get_media_subtype(mdia->mediaTrack->moov->mov , ref_track, 1)) {
					case GF_ISOM_SUBTYPE_HVC1:
					case GF_ISOM_SUBTYPE_HVC2:
					case GF_ISOM_SUBTYPE_HEV1:
					case GF_ISOM_SUBTYPE_HEV2:

						base_samp = gf_isom_get_sample(mdia->mediaTrack->moov->mov, ref_track, sampleNumber + mdia->mediaTrack->sample_count_at_seg_start, &di);
						if (base_samp && base_samp->data) {
							sample->data = gf_realloc(sample->data, sample->dataLength+base_samp->dataLength);
							memmove(sample->data + base_samp->dataLength, sample->data , sample->dataLength);
							memcpy(sample->data, base_samp->data, base_samp->dataLength);
							sample->dataLength += base_samp->dataLength;
						}
						if (base_samp) gf_isom_sample_del(&base_samp);
						Track_FindRef(mdia->mediaTrack, GF_ISOM_REF_BASE, &scal);
						break;
					}
				}
			}
		}

		sabt_ref = gf_isom_get_reference_count(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_SABT);
		if ((s32) sabt_ref > 0) {
			force_sei_inspect = GF_TRUE;
			for (i=0; i<sabt_ref; i++) {
				GF_ISOSample *tile_samp;
				gf_isom_get_reference(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_SABT, i+1, &ref_track);
				tile_samp = gf_isom_get_sample(mdia->mediaTrack->moov->mov, ref_track, sampleNumber + mdia->mediaTrack->sample_count_at_seg_start, &di);
				if (tile_samp  && tile_samp ->data) {
					sample->data = gf_realloc(sample->data, sample->dataLength+tile_samp->dataLength);
					memcpy(sample->data + sample->dataLength, tile_samp->data, tile_samp->dataLength);
					sample->dataLength += tile_samp->dataLength;
				}
				if (tile_samp) gf_isom_sample_del(&tile_samp);
			}
		}
	}

	if ( gf_isom_get_reference_count(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_TBAS) >= 1) {
		u32 ref_track;
		u32 idx = gf_list_find(mdia->information->sampleTable->SampleDescription->other_boxes, entry);
		GF_TrackBox *tbas;
		gf_isom_get_reference(mdia->mediaTrack->moov->mov, track_num, GF_ISOM_REF_TBAS, 1, &ref_track);
		tbas = (GF_TrackBox *)gf_list_get(mdia->mediaTrack->moov->trackList, ref_track-1);
		entry = gf_list_get(tbas->Media->information->sampleTable->SampleDescription->other_boxes, idx);
	}


	if (sample->IsRAP < SAP_TYPE_2) {
		if (mdia->information->sampleTable->no_sync_found || (!sample->IsRAP && check_cra_bla) ) {
			sample->IsRAP = is_sample_idr(sample, entry);
		}
	}
	if (!sample->IsRAP)
		rewrite_ps = GF_FALSE;

	if (extractor_mode != GF_ISOM_NALU_EXTRACT_LAYER_ONLY)
		insert_vdrd_code = GF_FALSE;

	if (entry->lhvc_config && entry->hevc_config)
		insert_vdrd_code = GF_FALSE;

	if (extractor_mode == GF_ISOM_NALU_EXTRACT_INSPECT) {
		if (!rewrite_ps && !rewrite_start_codes)
			return GF_OK;
	}

	if (!entry) return GF_BAD_PARAM;
	nal_unit_size_field = 0;
	/*if svc rewrite*/
	if (entry->svc_config && entry->svc_config->config)
		nal_unit_size_field = entry->svc_config->config->nal_unit_size;
	/*if mvc rewrite*/
	if (entry->mvc_config && entry->mvc_config->config)
		nal_unit_size_field = entry->mvc_config->config->nal_unit_size;

	/*if lhvc rewrite*/
	else if (entry->lhvc_config && entry->lhvc_config->config)  {
		is_hevc = GF_TRUE;
		nal_unit_size_field = entry->lhvc_config->config->nal_unit_size;
	}

	/*otherwise do nothing*/
	else if (!rewrite_ps && !rewrite_start_codes && !scal && !force_sei_inspect) {
		return GF_OK;
	}

	if (!nal_unit_size_field) {
		if (entry->avc_config) nal_unit_size_field = entry->avc_config->config->nal_unit_size;
		else if (entry->hevc_config || entry->lhvc_config ) {
			nal_unit_size_field = entry->lhvc_config ? entry->lhvc_config->config->nal_unit_size : entry->hevc_config->config->nal_unit_size;
			is_hevc = GF_TRUE;
		}
	}

	if (!nal_unit_size_field) return GF_ISOM_INVALID_FILE;

	dst_bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
	ps_bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
	src_bs = gf_bs_new(sample->data, sample->dataLength, GF_BITSTREAM_READ);
	if (!src_bs && sample->data) return GF_ISOM_INVALID_FILE;
	max_size = 4096;

	/*rewrite start code with NALU delim*/
	if (rewrite_start_codes) {

		if (insert_vdrd_code) {
			if (is_hevc) {
			} else {
				gf_bs_write_int(dst_bs, 1, 32);
				gf_bs_write_int(dst_bs, GF_AVC_NALU_VDRD , 8);
				insert_nalu_delim=0;
			}
		}

		if (insert_nalu_delim) {
			gf_bs_write_int(dst_bs, 1, 32);
			if (is_hevc) {
#ifndef GPAC_DISABLE_HEVC
				gf_bs_write_int(dst_bs, 0, 1);
				gf_bs_write_int(dst_bs, GF_HEVC_NALU_ACCESS_UNIT, 6);
				gf_bs_write_int(dst_bs, insert_vdrd_code ? 1 : 0, 6); //we should pick the layerID of the following nalus ...
				gf_bs_write_int(dst_bs, 1, 3); //nuh_temporal_id_plus1 - cannot be 0, we use 1 by default, and overwrite it if needed at the end

				/*pic-type - by default we signal all slice types possible*/
				gf_bs_write_int(dst_bs, 2, 3);
				gf_bs_write_int(dst_bs, 0, 5);
#endif
			} else {
				gf_bs_write_int(dst_bs, (sample->data[0] & 0x60) | GF_AVC_NALU_ACCESS_UNIT, 8);
				gf_bs_write_int(dst_bs, 0xF0 , 8); /*7 "all supported NALUs" (=111) + rbsp trailing (10000)*/;
			}
		}
	}

	if (rewrite_ps) {
		if (extractor_mode == GF_ISOM_NALU_EXTRACT_DEFAULT) {
			u32 i;

			if (scal) {
				for (i=0; i<scal->trackIDCount; i++) {
					GF_TrackBox *a_track = GetTrackbyID(mdia->mediaTrack->moov, scal->trackIDs[i]);
					GF_MPEGVisualSampleEntryBox *an_entry = NULL;
					if (a_track && a_track->Media && a_track->Media->information && a_track->Media->information->sampleTable && a_track->Media->information->sampleTable->SampleDescription)
						an_entry = (GF_MPEGVisualSampleEntryBox*)gf_list_get(a_track->Media->information->sampleTable->SampleDescription->other_boxes, 0);

					if (an_entry)
						nalu_merge_ps(ps_bs, rewrite_start_codes, nal_unit_size_field, an_entry, is_hevc);
				}
			}
		}
		nalu_merge_ps(ps_bs, rewrite_start_codes, nal_unit_size_field, entry, is_hevc);


		if (is_hevc) {
			/*little optimization if we are not asked to start codes: copy over the sample*/
			if (!rewrite_start_codes && !entry->lhvc_config && !scal) {
				if (ps_bs) {
					u8 nal_type = (sample->data[nal_unit_size_field] & 0x7E) >> 1;
					switch (nal_type) {
#ifndef GPAC_DISABLE_HEVC
					case GF_HEVC_NALU_VID_PARAM:
					case GF_HEVC_NALU_SEQ_PARAM:
					case GF_HEVC_NALU_PIC_PARAM:
						break;
#endif
					default:
						gf_bs_transfer(dst_bs, ps_bs);
						break;
					}
					gf_bs_del(ps_bs);
					ps_bs = NULL;
				}
				gf_bs_write_data(dst_bs, sample->data, sample->dataLength);
				gf_free(sample->data);
				sample->data = NULL;
				gf_bs_get_content(dst_bs, &sample->data, &sample->dataLength);
				gf_bs_del(src_bs);
				gf_bs_del(dst_bs);
				return GF_OK;
			}
		}
	}

	/*little optimization if we are not asked to rewrite extractors or start codes: copy over the sample*/
	if (!scal && !rewrite_start_codes && !rewrite_ps && !force_sei_inspect) {
		if (ps_bs)
		{
			gf_bs_transfer(dst_bs, ps_bs);
			gf_bs_del(ps_bs);
			ps_bs = NULL;
		}
		gf_bs_write_data(dst_bs, sample->data, sample->dataLength);
		gf_free(sample->data);
		sample->data = NULL;
		gf_bs_get_content(dst_bs, &sample->data, &sample->dataLength);
		gf_bs_del(src_bs);
		gf_bs_del(dst_bs);
		return GF_OK;
	}

	buffer = (char *)gf_malloc(sizeof(char)*max_size);

	while (gf_bs_available(src_bs)) {
		nal_size = gf_bs_read_int(src_bs, 8*nal_unit_size_field);
		if (gf_bs_get_position(src_bs) + nal_size > sample->dataLength) {
			GF_LOG(GF_LOG_WARNING, GF_LOG_CODING, ("Sample %u (size %u) rewrite: corrupted NAL Unit (size %u)\n", sampleNumber, sample->dataLength, nal_size));
			goto exit;
		}
		if (nal_size>max_size) {
			buffer = (char*) gf_realloc(buffer, sizeof(char)*nal_size);
			max_size = nal_size;
		}
		if (is_hevc) {
			nal_hdr = gf_bs_read_u16(src_bs);
			nal_type = (nal_hdr&0x7E00) >> 9;
		} else {
			nal_hdr = gf_bs_read_u8(src_bs);
			nal_type = nal_hdr & 0x1F;
		}

		if (is_hevc) {
			GF_BitStream *write_to_bs = dst_bs;
			if (ps_bs) {
				gf_bs_transfer(dst_bs, ps_bs);
				gf_bs_del(ps_bs);
				ps_bs = NULL;
			}

#ifndef GPAC_DISABLE_HEVC
			/*we already wrote this stuff*/
			if (nal_type==GF_HEVC_NALU_ACCESS_UNIT) {
				gf_bs_skip_bytes(src_bs, nal_size-2);
				continue;
			}
			switch (nal_type) {
			case 49:
				e = process_extractor(file, mdia, sampleNumber, sample->DTS, nal_size, nal_hdr, nal_unit_size_field, GF_TRUE, rewrite_ps, rewrite_start_codes, src_bs, dst_bs, extractor_mode);
				if (e) goto exit;
				break;

			case GF_HEVC_NALU_SLICE_TSA_N:
			case GF_HEVC_NALU_SLICE_STSA_N:
			case GF_HEVC_NALU_SLICE_TSA_R:
			case GF_HEVC_NALU_SLICE_STSA_R:
				if (temporal_id < (nal_hdr & 0x7))
					temporal_id = (nal_hdr & 0x7);
				/*rewrite nal*/
				gf_bs_read_data(src_bs, buffer, nal_size-2);
				if (rewrite_start_codes)
					gf_bs_write_u32(dst_bs, 1);
				else
					gf_bs_write_int(dst_bs, nal_size, 8*nal_unit_size_field);

				gf_bs_write_u16(dst_bs, nal_hdr);
				gf_bs_write_data(dst_bs, buffer, nal_size-2);
				break;

			case GF_HEVC_NALU_SLICE_BLA_W_LP:
			case GF_HEVC_NALU_SLICE_BLA_W_DLP:
			case GF_HEVC_NALU_SLICE_BLA_N_LP:
			case GF_HEVC_NALU_SLICE_IDR_W_DLP:
			case GF_HEVC_NALU_SLICE_IDR_N_LP:
			case GF_HEVC_NALU_SLICE_CRA:
				if (check_cra_bla && !sample->IsRAP) {
					if (ref_samp) gf_isom_sample_del(&ref_samp);
					if (src_bs) gf_bs_del(src_bs);
					if (ref_bs) gf_bs_del(ref_bs);
					if (dst_bs) gf_bs_del(dst_bs);
					if (buffer) gf_free(buffer);

					sample->IsRAP = sap_type_from_nal_type(nal_type);
					return gf_isom_nalu_sample_rewrite(mdia, sample, sampleNumber, entry);
				}
			default:
				/*rewrite nal*/
				if (nal_size<2) {
					GF_LOG(GF_LOG_ERROR, GF_LOG_CONTAINER, ("[iso file] Invalid nal size %d in sample %d\n", nal_type, sampleNumber));
					e = GF_NON_COMPLIANT_BITSTREAM;
					goto exit;
				}
				gf_bs_read_data(src_bs, buffer, nal_size-2);

				if (nal_type==GF_HEVC_NALU_SEI_SUFFIX) {
					if (!sei_suffix_bs) sei_suffix_bs = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
					write_to_bs = sei_suffix_bs;
				}

				if (rewrite_start_codes)
					gf_bs_write_u32(write_to_bs, 1);
				else
					gf_bs_write_int(write_to_bs, nal_size, 8*nal_unit_size_field);

				gf_bs_write_u16(write_to_bs, nal_hdr);
				gf_bs_write_data(write_to_bs, buffer, nal_size-2);
			}
#endif

			continue;
		}

		switch(nal_type) {
		case GF_AVC_NALU_ACCESS_UNIT:
			/*we already wrote this stuff*/
			gf_bs_skip_bytes(src_bs, nal_size-1);
			continue;
		case 31:
			e = process_extractor(file, mdia, sampleNumber, sample->DTS, nal_size, nal_hdr, nal_unit_size_field, GF_FALSE, rewrite_ps, rewrite_start_codes, src_bs, dst_bs, extractor_mode);
			if (e) goto exit;
			break;
		case GF_AVC_NALU_SEQ_PARAM:
		case GF_AVC_NALU_PIC_PARAM:
		case GF_AVC_NALU_SEQ_PARAM_EXT:
		case GF_AVC_NALU_SVC_SUBSEQ_PARAM:
			if (ps_bs) {
				gf_bs_del(ps_bs);
				ps_bs = NULL;
			}
		default:
			if (ps_bs) {
				gf_bs_transfer(dst_bs, ps_bs);
				gf_bs_del(ps_bs);
				ps_bs = NULL;
			}
			gf_bs_read_data(src_bs, buffer, nal_size-1);
			if (rewrite_start_codes)
				gf_bs_write_u32(dst_bs, 1);
			else
				gf_bs_write_int(dst_bs, nal_size, 8*nal_unit_size_field);

			gf_bs_write_u8(dst_bs, nal_hdr);
			gf_bs_write_data(dst_bs, buffer, nal_size-1);
		}
	}

	if (sei_suffix_bs) {
		gf_bs_transfer(dst_bs, sei_suffix_bs);
		gf_bs_del(sei_suffix_bs);
	}
	/*done*/
	gf_free(sample->data);
	sample->data = NULL;
	gf_bs_get_content(dst_bs, &sample->data, &sample->dataLength);

	/*rewrite temporal ID of AU Ddelim NALU (first one)*/
	if (rewrite_start_codes && is_hevc && temporal_id) {
		sample->data[6] = (sample->data[6] & 0xF8) | (temporal_id+1);
	}


exit:
	if (ref_samp) gf_isom_sample_del(&ref_samp);
	if (src_bs) gf_bs_del(src_bs);
	if (ref_bs) gf_bs_del(ref_bs);
	if (dst_bs) gf_bs_del(dst_bs);
	if (ps_bs)  gf_bs_del(ps_bs);
	if (buffer) gf_free(buffer);
	return e;
}
