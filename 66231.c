GF_Err txtc_Read(GF_Box *s, GF_BitStream *bs)
{
	u32 size, i;
	char *str;
	GF_TextConfigBox *ptr = (GF_TextConfigBox*)s;

	size = (u32) ptr->size;
	str = (char *)gf_malloc(sizeof(char)*size);

	i=0;

	while (size) {
		str[i] = gf_bs_read_u8(bs);
		size--;
		if (!str[i])
			break;
		i++;
	}
	if (i) ptr->config = gf_strdup(str);
	gf_free(str);

	return GF_OK;
}
