GF_Err flxs_dump(GF_Box *a, FILE * trace)
{
	GF_AdobeFlashAccessParamsBox *ptr = (GF_AdobeFlashAccessParamsBox *)a;
	if (!a) return GF_BAD_PARAM;
	gf_isom_box_dump_start(a, "AdobeFlashAccessParamsBox", trace);
	fprintf(trace, ">\n");
	if (ptr->metadata)
		fprintf(trace, "<FmrmsV2Metadata=\"%s\"/>\n", ptr->metadata);
	gf_isom_box_dump_done("AdobeFlashAccessParamsBox", a, trace);
	return GF_OK;
}
