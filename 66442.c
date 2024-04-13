GF_Err stvi_dump(GF_Box *a, FILE * trace)
{
	GF_StereoVideoBox *ptr = (GF_StereoVideoBox *) a;
	gf_isom_box_dump_start(a, "StereoVideoBox", trace);

	fprintf(trace, "single_view_allowed=\"%d\" stereo_scheme=\"%d\" ", ptr->single_view_allowed, ptr->stereo_scheme);
	dump_data_attribute(trace, "stereo_indication_type", ptr->stereo_indication_type, ptr->sit_len);
	fprintf(trace, ">\n");
	gf_isom_box_dump_done("StereoVideoBox", a, trace);
	return GF_OK;
}
