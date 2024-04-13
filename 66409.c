GF_Err pixi_dump(GF_Box *a, FILE * trace)
{
	u32 i;
	GF_PixelInformationPropertyBox *ptr = (GF_PixelInformationPropertyBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "PixelInformationPropertyBox", trace);
	fprintf(trace, ">\n");
	for (i = 0; i < ptr->num_channels; i++) {
		fprintf(trace, "<BitPerChannel bits_per_channel=\"%d\"/>\n", ptr->bits_per_channel[i]);
	}
	if (!ptr->size)
		fprintf(trace, "<BitPerChannel bits_per_channel=\"\"/>\n");

	gf_isom_box_dump_done("PixelInformationPropertyBox", a, trace);
	return GF_OK;
}
