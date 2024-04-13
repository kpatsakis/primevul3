GF_Err rtp_hnti_dump(GF_Box *a, FILE * trace)
{
	GF_RTPBox *p = (GF_RTPBox *)a;
	gf_isom_box_dump_start(a, "RTPMovieHintInformationBox", trace);
	fprintf(trace, "descriptionformat=\"%s\">\n", gf_4cc_to_str(p->subType));
	if (p->sdpText)
		fprintf(trace, "<!-- sdp text: %s -->\n", p->sdpText);
	gf_isom_box_dump_done("RTPMovieHintInformationBox", a, trace);
	return GF_OK;
}
