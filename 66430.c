GF_Err srpp_dump(GF_Box *a, FILE * trace)
{
	GF_SRTPProcessBox *ptr = (GF_SRTPProcessBox *) a;
	gf_isom_box_dump_start(a, "SRTPProcessBox", trace);

	fprintf(trace, "encryption_algorithm_rtp=\"%d\" encryption_algorithm_rtcp=\"%d\" integrity_algorithm_rtp=\"%d\" integrity_algorithm_rtcp=\"%d\">\n", ptr->encryption_algorithm_rtp, ptr->encryption_algorithm_rtcp, ptr->integrity_algorithm_rtp, ptr->integrity_algorithm_rtcp);

	if (ptr->info) gf_isom_box_dump(ptr->info, trace);
	if (ptr->scheme_type) gf_isom_box_dump(ptr->scheme_type, trace);

	gf_isom_box_dump_done("SRTPProcessBox", a, trace);
	return GF_OK;
}
