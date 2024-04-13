GF_Err iprp_dump(GF_Box *a, FILE * trace)
{
	GF_ItemPropertiesBox *ptr = (GF_ItemPropertiesBox *)a;
	gf_isom_box_dump_start(a, "ItemPropertiesBox", trace);
	fprintf(trace, ">\n");
	if (ptr->property_container) gf_isom_box_dump(ptr->property_container, trace);
	if (ptr->property_association) gf_isom_box_dump(ptr->property_association, trace);
	gf_isom_box_dump_done("ItemPropertiesBox", a, trace);
	return GF_OK;
}
