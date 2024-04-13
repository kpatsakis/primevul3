void ViewOD(GF_Terminal *term, u32 OD_ID, u32 number, const char *szURL)
{
	GF_MediaInfo odi;
	u32 i, j, count, d_enum,id;
	GF_Err e;
	NetStatCommand com;
	GF_ObjectManager *odm, *root_odm = gf_term_get_root_object(term);
	if (!root_odm) return;

	odm = NULL;
	if (!szURL && ((!OD_ID && (number == (u32)-1)) || ((OD_ID == (u32)(-1)) && !number))) {
		odm = root_odm;
		if ((gf_term_get_object_info(term, odm, &odi) != GF_OK)) odm=NULL;
	} else {
		count = gf_term_get_object_count(term, root_odm);
		for (i=0; i<count; i++) {
			odm = gf_term_get_object(term, root_odm, i);
			if (!odm) break;
			if (gf_term_get_object_info(term, odm, &odi) == GF_OK) {
				if (szURL && strstr(odi.service_url, szURL)) break;
				if ((number == (u32)(-1)) && odi.od && (odi.od->objectDescriptorID == OD_ID)) break;
				else if (i == (u32)(number-1)) break;
			}
			odm = NULL;
		}
	}
	if (!odm) {
		if (szURL) fprintf(stderr, "cannot find OD for URL %s\n", szURL);
		if (number == (u32)-1) fprintf(stderr, "cannot find OD with ID %d\n", OD_ID);
		else fprintf(stderr, "cannot find OD with number %d\n", number);
		return;
	}
	if (!odi.od) {
		if (number == (u32)-1) fprintf(stderr, "Object %d not attached yet\n", OD_ID);
		else fprintf(stderr, "Object #%d not attached yet\n", number);
		return;
	}

	if (!odi.od) {
		fprintf(stderr, "Service not attached\n");
		return;
	}

	if (odi.od->tag==GF_ODF_IOD_TAG) {
		fprintf(stderr, "InitialObjectDescriptor %d\n", odi.od->objectDescriptorID);
		fprintf(stderr, "Profiles and Levels: Scene %x - Graphics %x - Visual %x - Audio %x - OD %x\n",
		        odi.scene_pl, odi.graphics_pl, odi.visual_pl, odi.audio_pl, odi.OD_pl);
		fprintf(stderr, "Inline Profile Flag %d\n", odi.inline_pl);
	} else {
		fprintf(stderr, "ObjectDescriptor %d\n", odi.od->objectDescriptorID);
	}

	fprintf(stderr, "Object Duration: ");
	if (odi.duration) {
		PrintTime((u32) (odi.duration*1000));
	} else {
		fprintf(stderr, "unknown");
	}
	fprintf(stderr, "\n");

	fprintf(stderr, "Service Handler: %s\n", odi.service_handler);
	fprintf(stderr, "Service URL: %s\n", odi.service_url);

	if (odi.codec_name) {
		Float avg_dec_time;
		switch (odi.od_type) {
		case GF_STREAM_VISUAL:
			fprintf(stderr, "Video Object: Width %d - Height %d\r\n", odi.width, odi.height);
			fprintf(stderr, "Media Codec: %s\n", odi.codec_name);
			if (odi.par) fprintf(stderr, "Pixel Aspect Ratio: %d:%d\n", (odi.par>>16)&0xFF, (odi.par)&0xFF);
			break;
		case GF_STREAM_AUDIO:
			fprintf(stderr, "Audio Object: Sample Rate %d - %d channels\r\n", odi.sample_rate, odi.num_channels);
			fprintf(stderr, "Media Codec: %s\n", odi.codec_name);
			break;
		case GF_STREAM_SCENE:
		case GF_STREAM_PRIVATE_SCENE:
			if (odi.width && odi.height) {
				fprintf(stderr, "Scene Description - Width %d - Height %d\n", odi.width, odi.height);
			} else {
				fprintf(stderr, "Scene Description - no size specified\n");
			}
			fprintf(stderr, "Scene Codec: %s\n", odi.codec_name);
			break;
		case GF_STREAM_TEXT:
			if (odi.width && odi.height) {
				fprintf(stderr, "Text Object: Width %d - Height %d\n", odi.width, odi.height);
			} else {
				fprintf(stderr, "Text Object: No size specified\n");
			}
			fprintf(stderr, "Text Codec %s\n", odi.codec_name);
			break;
		}

		avg_dec_time = 0;
		if (odi.nb_dec_frames) {
			avg_dec_time = (Float) odi.total_dec_time;
			avg_dec_time /= odi.nb_dec_frames;
		}
		fprintf(stderr, "\tBitrate over last second: %d kbps\n\tMax bitrate over one second: %d kbps\n\tAverage Decoding Time %.2f us %d max)\n\tTotal decoded frames %d\n",
		        (u32) odi.avg_bitrate/1024, odi.max_bitrate/1024, avg_dec_time, odi.max_dec_time, odi.nb_dec_frames);
	}
	if (odi.protection) fprintf(stderr, "Encrypted Media%s\n", (odi.protection==2) ? " NOT UNLOCKED" : "");

	count = gf_list_count(odi.od->ESDescriptors);
	fprintf(stderr, "%d streams in OD\n", count);
	for (i=0; i<count; i++) {
		GF_ESD *esd = (GF_ESD *) gf_list_get(odi.od->ESDescriptors, i);

		fprintf(stderr, "\nStream ID %d - Clock ID %d\n", esd->ESID, esd->OCRESID);
		if (esd->dependsOnESID) fprintf(stderr, "\tDepends on Stream ID %d for decoding\n", esd->dependsOnESID);

		switch (esd->decoderConfig->streamType) {
		case GF_STREAM_OD:
			fprintf(stderr, "\tOD Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_OCR:
			fprintf(stderr, "\tOCR Stream\n");
			break;
		case GF_STREAM_SCENE:
			fprintf(stderr, "\tScene Description Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_VISUAL:
			fprintf(stderr, "\tVisual Stream - media type: %s", gf_esd_get_textual_description(esd));
			break;
		case GF_STREAM_AUDIO:
			fprintf(stderr, "\tAudio Stream - media type: %s", gf_esd_get_textual_description(esd));
			break;
		case GF_STREAM_MPEG7:
			fprintf(stderr, "\tMPEG-7 Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_IPMP:
			fprintf(stderr, "\tIPMP Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_OCI:
			fprintf(stderr, "\tOCI Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_MPEGJ:
			fprintf(stderr, "\tMPEGJ Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_INTERACT:
			fprintf(stderr, "\tUser Interaction Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		case GF_STREAM_TEXT:
			fprintf(stderr, "\tStreaming Text Stream - version %d\n", esd->decoderConfig->objectTypeIndication);
			break;
		default:
			fprintf(stderr, "\tUnknown Stream\n");
			break;
		}

		fprintf(stderr, "\tBuffer Size %d\n\tAverage Bitrate %d bps\n\tMaximum Bitrate %d bps\n", esd->decoderConfig->bufferSizeDB, esd->decoderConfig->avgBitrate, esd->decoderConfig->maxBitrate);
		if (esd->slConfig->predefined==SLPredef_SkipSL) {
			fprintf(stderr, "\tNot using MPEG-4 Synchronization Layer\n");
		} else {
			fprintf(stderr, "\tStream Clock Resolution %d\n", esd->slConfig->timestampResolution);
		}
		if (esd->URLString) fprintf(stderr, "\tStream Location: %s\n", esd->URLString);

		/*check language*/
		if (esd->langDesc) {
			s32 lang_idx;
			char lan[4];
			lan[0] = esd->langDesc->langCode>>16;
			lan[1] = (esd->langDesc->langCode>>8)&0xFF;
			lan[2] = (esd->langDesc->langCode)&0xFF;
			lan[3] = 0;

			lang_idx = gf_lang_find(lan);
			if (lang_idx>=0) {
				fprintf(stderr, "\tStream Language: %s\n", gf_lang_get_name(lang_idx));
			}
		}
	}
	fprintf(stderr, "\n");
	/*check OCI (not everything interests us) - FIXME: support for unicode*/
	count = gf_list_count(odi.od->OCIDescriptors);
	if (count) {
		fprintf(stderr, "%d Object Content Information descriptors in OD\n", count);
		for (i=0; i<count; i++) {
			GF_Descriptor *desc = (GF_Descriptor *) gf_list_get(odi.od->OCIDescriptors, i);
			switch (desc->tag) {
			case GF_ODF_SEGMENT_TAG:
			{
				GF_Segment *sd = (GF_Segment *) desc;
				fprintf(stderr, "Segment Descriptor: Name: %s - start time %g sec - duration %g sec\n", sd->SegmentName, sd->startTime, sd->Duration);
			}
			break;
			case GF_ODF_CC_NAME_TAG:
			{
				GF_CC_Name *ccn = (GF_CC_Name *)desc;
				fprintf(stderr, "Content Creators:\n");
				for (j=0; j<gf_list_count(ccn->ContentCreators); j++) {
					GF_ContentCreatorInfo *ci = (GF_ContentCreatorInfo *) gf_list_get(ccn->ContentCreators, j);
					if (!ci->isUTF8) continue;
					fprintf(stderr, "\t%s\n", ci->contentCreatorName);
				}
			}
			break;

			case GF_ODF_SHORT_TEXT_TAG:
			{
				GF_ShortTextual *std = (GF_ShortTextual *)desc;
				fprintf(stderr, "Description:\n\tEvent: %s\n\t%s\n", std->eventName, std->eventText);
			}
			break;
			default:
				break;
			}
		}
		fprintf(stderr, "\n");
	}

	switch (odi.status) {
	case 0:
		fprintf(stderr, "Stopped - ");
		break;
	case 1:
		fprintf(stderr, "Playing - ");
		break;
	case 2:
		fprintf(stderr, "Paused - ");
		break;
	case 3:
		fprintf(stderr, "Not setup yet\n");
		return;
	default:
		fprintf(stderr, "Setup Failed\n");
		return;
	}
	if (odi.buffer>=0) fprintf(stderr, "Buffer: %d ms - ", odi.buffer);
	else fprintf(stderr, "Not buffering - ");
	fprintf(stderr, "Clock drift: %d ms\n", odi.clock_drift);
	if (odi.db_unit_count) fprintf(stderr, "%d AU in DB\n", odi.db_unit_count);
	if (odi.cb_max_count) fprintf(stderr, "Composition Buffer: %d CU (%d max)\n", odi.cb_unit_count, odi.cb_max_count);
	fprintf(stderr, "\n");

	if (odi.owns_service) {
		const char *url;
		u32 done, total, bps;
		d_enum = 0;
		while (gf_term_get_download_info(term, odm, &d_enum, &url, NULL, &done, &total, &bps)) {
			if (d_enum==1) fprintf(stderr, "Current Downloads in service:\n");
			if (done && total) {
				fprintf(stderr, "%s: %d / %d bytes (%.2f %%) - %.2f kBps\n", url, done, total, (100.0f*done)/total, ((Float)bps)/1024.0f);
			} else {
				fprintf(stderr, "%s: %.2f kbps\n", url, ((Float)8*bps)/1024.0f);
			}
		}
		if (!d_enum) fprintf(stderr, "No Downloads in service\n");
		fprintf(stderr, "\n");
	}
	d_enum = 0;
	while (gf_term_get_channel_net_info(term, odm, &d_enum, &id, &com, &e)) {
		if (e) continue;
		if (!com.bw_down && !com.bw_up) continue;

		fprintf(stderr, "Stream ID %d statistics:\n", id);
		if (com.multiplex_port) {
			fprintf(stderr, "\tMultiplex Port %d - multiplex ID %d\n", com.multiplex_port, com.port);
		} else {
			fprintf(stderr, "\tPort %d\n", com.port);
		}
		fprintf(stderr, "\tPacket Loss Percentage: %.4f\n", com.pck_loss_percentage);
		fprintf(stderr, "\tDown Bandwidth: %d bps\n", com.bw_down);
		if (com.bw_up) fprintf(stderr, "\tUp Bandwidth: %d bps\n", com.bw_up);
		if (com.ctrl_port) {
			if (com.multiplex_port) {
				fprintf(stderr, "\tControl Multiplex Port: %d - Control Multiplex ID %d\n", com.multiplex_port, com.ctrl_port);
			} else {
				fprintf(stderr, "\tControl Port: %d\n", com.ctrl_port);
			}
			fprintf(stderr, "\tDown Bandwidth: %d bps\n", com.ctrl_bw_down);
			fprintf(stderr, "\tUp Bandwidth: %d bps\n", com.ctrl_bw_up);
		}
		fprintf(stderr, "\n");
	}
}
