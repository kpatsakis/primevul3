GF_Err dac3_dump(GF_Box *a, FILE * trace)
{
	GF_AC3ConfigBox *p = (GF_AC3ConfigBox *)a;

	if (p->cfg.is_ec3) {
		u32 i;
		a->type = GF_ISOM_BOX_TYPE_DEC3;
		gf_isom_box_dump_start(a, "EC3SpecificBox", trace);
		a->type = GF_ISOM_BOX_TYPE_DAC3;
		fprintf(trace, "nb_streams=\"%d\" data_rate=\"%d\">\n", p->cfg.nb_streams, p->cfg.brcode);
		for (i=0; i<p->cfg.nb_streams; i++) {
			fprintf(trace, "<EC3StreamConfig fscod=\"%d\" bsid=\"%d\" bsmod=\"%d\" acmod=\"%d\" lfon=\"%d\" num_sub_dep=\"%d\" chan_loc=\"%d\"/>\n",
			        p->cfg.streams[i].fscod, p->cfg.streams[i].bsid, p->cfg.streams[i].bsmod, p->cfg.streams[i].acmod, p->cfg.streams[i].lfon, p->cfg.streams[i].nb_dep_sub, p->cfg.streams[i].chan_loc);
		}
		gf_isom_box_dump_done("EC3SpecificBox", a, trace);
	} else {
		gf_isom_box_dump_start(a, "AC3SpecificBox", trace);
		fprintf(trace, "fscod=\"%d\" bsid=\"%d\" bsmod=\"%d\" acmod=\"%d\" lfon=\"%d\" bit_rate_code=\"%d\">\n",
		        p->cfg.streams[0].fscod, p->cfg.streams[0].bsid, p->cfg.streams[0].bsmod, p->cfg.streams[0].acmod, p->cfg.streams[0].lfon, p->cfg.brcode);
		gf_isom_box_dump_done("AC3SpecificBox", a, trace);
	}
	return GF_OK;
}
