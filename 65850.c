GF_Err lsrc_Read(GF_Box *s, GF_BitStream *bs)
{
	GF_LASERConfigurationBox *ptr = (GF_LASERConfigurationBox *)s;
	ptr->hdr_size = (u32) ptr->size;
	ptr->hdr = gf_malloc(sizeof(char)*ptr->hdr_size);
	gf_bs_read_data(bs, ptr->hdr, ptr->hdr_size);
	return GF_OK;
}
