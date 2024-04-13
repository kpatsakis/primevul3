GF_Err hinf_Write(GF_Box *s, GF_BitStream *bs)
{
	if (!s) return GF_BAD_PARAM;
	return gf_isom_box_write_header(s, bs);
}
