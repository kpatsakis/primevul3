static u8 avc_get_sar_idx(u32 w, u32 h)
{
	u32 i;
	for (i=0; i<14; i++) {
		if ((avc_sar[i].w==w) && (avc_sar[i].h==h)) return i;
	}
	return 0xFF;
}
