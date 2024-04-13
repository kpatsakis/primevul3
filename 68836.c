static void get_v2rx_desc_bnum(u32 bnum_flag, int *out_bnum)
{
	*out_bnum = hnae_get_field(bnum_flag,
				   HNS_RXD_BUFNUM_M, HNS_RXD_BUFNUM_S) + 1;
}
