GF_Err sdp_dump(GF_Box *a, FILE * trace)
{
	GF_SDPBox *p = (GF_SDPBox *)a;
	gf_isom_box_dump_start(a, "SDPBox", trace);
	fprintf(trace, ">\n");
	if (p->sdpText)
		fprintf(trace, "<!-- sdp text: %s -->\n", p->sdpText);
	gf_isom_box_dump_done("SDPBox", a, trace);
	return GF_OK;
}
