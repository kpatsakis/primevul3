GF_Err sdp_Size(GF_Box *s)
{
	GF_SDPBox *ptr = (GF_SDPBox *)s;
	ptr->size += strlen(ptr->sdpText);
	return GF_OK;
}
