GF_Err gf_isom_dump_hint_sample(GF_ISOFile *the_file, u32 trackNumber, u32 SampleNum, FILE * trace)
{
	GF_ISOSample *tmp;
	GF_HintSampleEntryBox *entry;
	u32 descIndex, count, count2, i;
	GF_Err e=GF_OK;
	GF_BitStream *bs;
	GF_HintSample *s;
	GF_TrackBox *trak;
	GF_RTPPacket *pck;
	char *szName;

	trak = gf_isom_get_track_from_file(the_file, trackNumber);
	if (!trak || !IsHintTrack(trak)) return GF_BAD_PARAM;

	tmp = gf_isom_get_sample(the_file, trackNumber, SampleNum, &descIndex);
	if (!tmp) return GF_BAD_PARAM;

	e = Media_GetSampleDesc(trak->Media, descIndex, (GF_SampleEntryBox **) &entry, &count);
	if (e) {
		gf_isom_sample_del(&tmp);
		return e;
	}

	switch (entry->type) {
	case GF_ISOM_BOX_TYPE_RTP_STSD:
	case GF_ISOM_BOX_TYPE_SRTP_STSD:
	case GF_ISOM_BOX_TYPE_RRTP_STSD:
		szName = "RTP";
		break;
	case GF_ISOM_BOX_TYPE_RTCP_STSD:
		szName = "RCTP";
		break;
	case GF_ISOM_BOX_TYPE_FDP_STSD:
		szName = "FDP";
		break;
	default:
		gf_isom_sample_del(&tmp);
		return GF_NOT_SUPPORTED;
	}

	bs = gf_bs_new(tmp->data, tmp->dataLength, GF_BITSTREAM_READ);
	s = gf_isom_hint_sample_new(entry->type);
	s->trackID = trak->Header->trackID;
	s->sampleNumber = SampleNum;

	gf_isom_hint_sample_read(s, bs, tmp->dataLength);
	gf_bs_del(bs);

	count = gf_list_count(s->packetTable);

	fprintf(trace, "<%sHintSample SampleNumber=\"%d\" DecodingTime=\""LLD"\" RandomAccessPoint=\"%d\" PacketCount=\"%u\" reserved=\"%u\">\n", szName, SampleNum, LLD_CAST tmp->DTS, tmp->IsRAP, s->packetCount, s->reserved);

	if (s->hint_subtype==GF_ISOM_BOX_TYPE_FDP_STSD) {
		e = gf_isom_box_dump((GF_Box*) s, trace);
		goto err_exit;
	}

	if (s->packetCount != count) {
		fprintf(trace, "<!-- WARNING: Broken %s hint sample, %d entries indicated but only %d parsed -->\n", szName, s->packetCount, count);
	}


	for (i=0; i<count; i++) {
		pck = (GF_RTPPacket *)gf_list_get(s->packetTable, i);

		if (pck->hint_subtype==GF_ISOM_BOX_TYPE_RTCP_STSD) {
			GF_RTCPPacket *rtcp_pck = (GF_RTCPPacket *) pck;
			fprintf(trace, "<RTCPHintPacket PacketNumber=\"%d\" V=\"%d\" P=\"%d\" Count=\"%d\" PayloadType=\"%d\" ",
		        i+1,  rtcp_pck->Version, rtcp_pck->Padding, rtcp_pck->Count, rtcp_pck->PayloadType);

			if (rtcp_pck->data) dump_data_attribute(trace, "payload", (char*)rtcp_pck->data, rtcp_pck->length);
			fprintf(trace, ">\n");
			fprintf(trace, "</RTCPHintPacket>\n");

		} else {
			fprintf(trace, "<RTPHintPacket PacketNumber=\"%d\" P=\"%d\" X=\"%d\" M=\"%d\" PayloadType=\"%d\"",
		        i+1,  pck->P_bit, pck->X_bit, pck->M_bit, pck->payloadType);

			fprintf(trace, " SequenceNumber=\"%d\" RepeatedPacket=\"%d\" DropablePacket=\"%d\" RelativeTransmissionTime=\"%d\" FullPacketSize=\"%d\">\n",
		        pck->SequenceNumber, pck->R_bit, pck->B_bit, pck->relativeTransTime, gf_isom_hint_rtp_length(pck));


			count2 = gf_list_count(pck->TLV);
			if (count2) {
				fprintf(trace, "<PrivateExtensionTable EntryCount=\"%d\">\n", count2);
				gf_isom_box_array_dump(pck->TLV, trace);
				fprintf(trace, "</PrivateExtensionTable>\n");
			}
			count2 = gf_list_count(pck->DataTable);
			if (count2) {
				fprintf(trace, "<PacketDataTable EntryCount=\"%d\">\n", count2);
				DTE_Dump(pck->DataTable, trace);
				fprintf(trace, "</PacketDataTable>\n");
			}
			fprintf(trace, "</RTPHintPacket>\n");
		}
	}

err_exit:
	fprintf(trace, "</%sHintSample>\n", szName);
	gf_isom_sample_del(&tmp);
	gf_isom_hint_sample_del(s);
	return e;
}
