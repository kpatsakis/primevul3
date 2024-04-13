GF_Err btrt_Read(GF_Box *s, GF_BitStream *bs)
{
	GF_BitRateBox *ptr = (GF_BitRateBox *)s;
	ptr->bufferSizeDB = gf_bs_read_u32(bs);
	ptr->maxBitrate = gf_bs_read_u32(bs);
	ptr->avgBitrate = gf_bs_read_u32(bs);
	return GF_OK;
}
