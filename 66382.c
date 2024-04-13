GF_Err meta_dump(GF_Box *a, FILE * trace)
{
	GF_MetaBox *p;
	p = (GF_MetaBox *)a;
	gf_isom_box_dump_start(a, "MetaBox", trace);
	fprintf(trace, ">\n");

	if (p->handler) gf_isom_box_dump(p->handler, trace);
	if (p->primary_resource) gf_isom_box_dump(p->primary_resource, trace);
	if (p->file_locations) gf_isom_box_dump(p->file_locations, trace);
	if (p->item_locations) gf_isom_box_dump(p->item_locations, trace);
	if (p->protections) gf_isom_box_dump(p->protections, trace);
	if (p->item_infos) gf_isom_box_dump(p->item_infos, trace);
	if (p->IPMP_control) gf_isom_box_dump(p->IPMP_control, trace);
	if (p->item_refs) gf_isom_box_dump(p->item_refs, trace);
	if (p->item_props) gf_isom_box_dump(p->item_props, trace);
	gf_isom_box_dump_done("MetaBox", a, trace);
	return GF_OK;
}
