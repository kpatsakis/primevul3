static void stsd_switch_box(GF_BitStream *bs, GF_Box *box, GF_UnknownBox *a, u8 **data, u32 *data_size, u32 *EntryType, GF_SampleDescriptionBox *stsd, u32 stsd_idx)
{
	if (gf_bs_available(bs)) {
		GF_Err e;
		u32 count_subb;
		gf_bs_set_cookie(bs, GF_ISOM_BS_COOKIE_NO_LOGS);
		e = gf_isom_box_array_read(box, bs);
		count_subb = box->child_boxes ? gf_list_count(box->child_boxes) : 0;
		if (count_subb && !e) {
			u32 i;
			GF_BitStream *new_dsi = gf_bs_new(NULL, 0, GF_BITSTREAM_WRITE);
			//serialize all unknown boxes
			for (i=0; i<count_subb; i++) {
				GF_UnknownBox *priv = gf_list_get(box->child_boxes, i);
				if (priv->type != GF_ISOM_BOX_TYPE_UNKNOWN) continue;
				gf_bs_write_u32(new_dsi, 8 + priv->dataSize);
				gf_bs_write_u32(new_dsi, priv->original_4cc);
				gf_bs_write_data(new_dsi, priv->data, priv->dataSize);
				gf_list_rem(box->child_boxes, i);
				gf_isom_box_del((GF_Box*)priv);
				count_subb--;
				i--;
			}
			if (*data) gf_free(*data);
			gf_bs_get_content(new_dsi, data, data_size);
			gf_bs_del(new_dsi);
		} else {
			*data_size = 0;
		}
	}
	gf_bs_del(bs);
	if (!*data_size && *data) {
		gf_free(*data);
		*data = NULL;
	}
	box->size = 0;
	*EntryType = a->original_4cc;
	gf_list_rem(stsd->child_boxes, stsd_idx);
	gf_isom_box_del((GF_Box *)a);
	gf_list_insert(stsd->child_boxes, box, stsd_idx);
}