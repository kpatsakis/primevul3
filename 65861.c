GF_Err mdat_Write(GF_Box *s, GF_BitStream *bs)
{
	GF_Err e;
	GF_MediaDataBox *ptr = (GF_MediaDataBox *)s;
	e = gf_isom_box_write_header(s, bs);
	if (e) return e;

	if (ptr->data) {
		gf_bs_write_data(bs, ptr->data, (u32) ptr->dataSize);
	}
	return GF_OK;
}
