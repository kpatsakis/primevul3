GF_Err tpay_Read(GF_Box *s, GF_BitStream *bs)
{
	GF_TPAYBox *ptr = (GF_TPAYBox *)s;
	ptr->nbBytes = gf_bs_read_u32(bs);
	return GF_OK;
}
