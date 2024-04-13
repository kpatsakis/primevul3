GF_Err ispe_dump(GF_Box *a, FILE * trace)
{
	GF_ImageSpatialExtentsPropertyBox *ptr = (GF_ImageSpatialExtentsPropertyBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "ImageSpatialExtentsPropertyBox", trace);
	fprintf(trace, "image_width=\"%d\" image_height=\"%d\">\n", ptr->image_width, ptr->image_height);
	gf_isom_box_dump_done("ImageSpatialExtentsPropertyBox", a, trace);
	return GF_OK;
}
