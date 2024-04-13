GF_Err ccst_dump(GF_Box *a, FILE * trace)
{
	GF_CodingConstraintsBox *p = (GF_CodingConstraintsBox *)a;
	gf_isom_box_dump_start(a, "CodingConstraintsBox", trace);
	fprintf(trace, "all_ref_pics_intra=\"%d\" intra_pred_used=\"%d\" max_ref_per_pic=\"%d\" reserved=\"%d\">\n", p->all_ref_pics_intra, p->intra_pred_used, p->max_ref_per_pic, p->reserved);
	gf_isom_box_dump_done("CodingConstraintsBox", a, trace);
	return GF_OK;
}
