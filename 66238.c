GF_Err udta_Size(GF_Box *s)
{
	GF_Err e;
	u32 i;
	GF_UserDataMap *map;
	GF_UserDataBox *ptr = (GF_UserDataBox *)s;

	i=0;
	while ((map = (GF_UserDataMap *)gf_list_enum(ptr->recordList, &i))) {
		e = gf_isom_box_array_size(s, map->other_boxes);
		if (e) return e;
	}
	return GF_OK;
}
