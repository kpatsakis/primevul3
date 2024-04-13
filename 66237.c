GF_Err udta_Read(GF_Box *s, GF_BitStream *bs)
{
	return gf_isom_box_array_read(s, bs, udta_AddBox);
}
