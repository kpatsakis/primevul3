GF_Err udta_Write(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	u32 i;
	GF_UserDataMap *map;
	GF_UserDataBox *ptr = (GF_UserDataBox *)s;

	e = gf_isom_box_write_header(s, bs);
	if (e) return e;
	i=0;
	while ((map = (GF_UserDataMap *)gf_list_enum(ptr->recordList, &i))) {
		e = gf_isom_box_array_write(s, map->other_boxes, bs);
		if (e) return e;
	}
	return GF_OK;
}
