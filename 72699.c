GF_Err set_file_udta(GF_ISOFile *dest, u32 tracknum, u32 udta_type, char *src, Bool is_box_array)
{
	char *data = NULL;
	GF_Err res = GF_OK;
	u32 size;
	bin128 uuid;
	memset(uuid, 0 , 16);

	if (!udta_type && !is_box_array) return GF_BAD_PARAM;

	if (!src) {
		return gf_isom_remove_user_data(dest, tracknum, udta_type, uuid);
	}

#ifndef GPAC_DISABLE_CORE_TOOLS
	if (!strnicmp(src, "base64", 6)) {
		src += 7;
		size = (u32) strlen(src);
		data = gf_malloc(sizeof(char) * size);
		size = gf_base64_decode(src, size, data, size);
	} else
#endif
	{
		FILE *t = gf_fopen(src, "rb");
		if (!t) return GF_IO_ERR;
		fseek(t, 0, SEEK_END);
		size = ftell(t);
		fseek(t, 0, SEEK_SET);
		data = gf_malloc(sizeof(char)*size);
		if (size != fread(data, 1, size, t) ) {
			gf_free(data);
			gf_fclose(t);
			return GF_IO_ERR;
		}
		gf_fclose(t);
	}

	if (size && data) {
		if (is_box_array) {
			res = gf_isom_add_user_data_boxes(dest, tracknum, data, size);
		} else {
			res = gf_isom_add_user_data(dest, tracknum, udta_type, uuid, data, size);
		}
		gf_free(data);
	}
	return res;
}
