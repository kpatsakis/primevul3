GF_Err ipro_dump(GF_Box *a, FILE * trace)
{
	GF_ItemProtectionBox *p = (GF_ItemProtectionBox *)a;
	gf_isom_box_dump_start(a, "ItemProtectionBox", trace);
	fprintf(trace, ">\n");
	gf_isom_box_array_dump(p->protection_information, trace);
	gf_isom_box_dump_done("ItemProtectionBox", a, trace);
	return GF_OK;
}
