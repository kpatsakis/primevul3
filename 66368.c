GF_Err irot_dump(GF_Box *a, FILE * trace)
{
	GF_ImageRotationBox *ptr = (GF_ImageRotationBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "ImageRotationBox", trace);
	fprintf(trace, "angle=\"%d\">\n", (ptr->angle*90));
	gf_isom_box_dump_done("ImageRotationBox", a, trace);
	return GF_OK;
}
