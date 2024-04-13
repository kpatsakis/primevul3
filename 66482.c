GF_Err udta_dump(GF_Box *a, FILE * trace)
{
	GF_UserDataBox *p;
	GF_UserDataMap *map;
	u32 i;

	p = (GF_UserDataBox *)a;
	gf_isom_box_dump_start(a, "UserDataBox", trace);
	fprintf(trace, ">\n");

	i=0;
	while ((map = (GF_UserDataMap *)gf_list_enum(p->recordList, &i))) {
		gf_isom_box_array_dump(map->other_boxes, trace);
	}
	gf_isom_box_dump_done("UserDataBox", a, trace);
	return GF_OK;
}
