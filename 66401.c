GF_Err pasp_dump(GF_Box *a, FILE * trace)
{
	GF_PixelAspectRatioBox *ptr = (GF_PixelAspectRatioBox*)a;
	gf_isom_box_dump_start(a, "PixelAspectRatioBox", trace);
	fprintf(trace, "hSpacing=\"%d\" vSpacing=\"%d\" >\n", ptr->hSpacing, ptr->vSpacing);
	gf_isom_box_dump_done("PixelAspectRatioBox", a, trace);
	return GF_OK;
}
