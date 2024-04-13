GF_Err ghnt_dump(GF_Box *a, FILE * trace)
{
	char *name;
	GF_HintSampleEntryBox *p;
	p = (GF_HintSampleEntryBox *)a;

	if (a->type == GF_ISOM_BOX_TYPE_RTP_STSD) {
		name = "RTPHintSampleEntryBox";
	} else if (a->type == GF_ISOM_BOX_TYPE_SRTP_STSD) {
		name = "SRTPHintSampleEntryBox";
	} else if (a->type == GF_ISOM_BOX_TYPE_FDP_STSD) {
		name = "FDPHintSampleEntryBox";
	} else if (a->type == GF_ISOM_BOX_TYPE_RRTP_STSD) {
		name = "RTPReceptionHintSampleEntryBox";
	} else if (a->type == GF_ISOM_BOX_TYPE_RTCP_STSD) {
		name = "RTCPReceptionHintSampleEntryBox";
	} else {
		name = "GenericHintSampleEntryBox";
	}
	gf_isom_box_dump_start(a, name, trace);
	fprintf(trace, "DataReferenceIndex=\"%d\" HintTrackVersion=\"%d\" LastCompatibleVersion=\"%d\"", p->dataReferenceIndex, p->HintTrackVersion, p->LastCompatibleVersion);
	if ((a->type == GF_ISOM_BOX_TYPE_RTP_STSD) || (a->type == GF_ISOM_BOX_TYPE_SRTP_STSD) || (a->type == GF_ISOM_BOX_TYPE_RRTP_STSD) || (a->type == GF_ISOM_BOX_TYPE_RTCP_STSD)) {
		fprintf(trace, " MaxPacketSize=\"%d\"", p->MaxPacketSize);
	} else if (a->type == GF_ISOM_BOX_TYPE_FDP_STSD) {
		fprintf(trace, " partition_entry_ID=\"%d\" FEC_overhead=\"%d\"", p->partition_entry_ID, p->FEC_overhead);
	}
	fprintf(trace, ">\n");

	gf_isom_box_dump_done(name, a, trace);
	return GF_OK;
}
