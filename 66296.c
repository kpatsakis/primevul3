GF_Err colr_dump(GF_Box *a, FILE * trace)
{
	GF_ColourInformationBox *ptr = (GF_ColourInformationBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "ColourInformationBox", trace);
	fprintf(trace, "colour_type=\"%s\" colour_primaries=\"%d\" transfer_characteristics=\"%d\" matrix_coefficients=\"%d\" full_range_flag=\"%d\">\n", gf_4cc_to_str(ptr->colour_type), ptr->colour_primaries, ptr->transfer_characteristics, ptr->matrix_coefficients, ptr->full_range_flag);
	gf_isom_box_dump_done("ColourInformationBox", a, trace);
	return GF_OK;
}
