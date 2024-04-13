GF_Err ssix_Read(GF_Box *s, GF_BitStream *bs)
{
	u32 i,j;
	GF_SubsegmentIndexBox *ptr = (GF_SubsegmentIndexBox*)s;

	if (ptr->size < 4) return GF_BAD_PARAM;
	ptr->subsegment_count = gf_bs_read_u32(bs);
	ptr->size -= 4;
	ptr->subsegments = gf_malloc(ptr->subsegment_count*sizeof(GF_Subsegment));
	for (i = 0; i < ptr->subsegment_count; i++) {
		GF_Subsegment *subseg = &ptr->subsegments[i];
		if (ptr->size < 4) return GF_BAD_PARAM;
		subseg->range_count = gf_bs_read_u32(bs);
		ptr->size -= 4;
		if (ptr->size < subseg->range_count*4) return GF_BAD_PARAM;
		subseg->levels = gf_malloc(sizeof(u8)*subseg->range_count);
		subseg->range_sizes = gf_malloc(sizeof(u32)*subseg->range_count);
		for (j = 0; j < subseg->range_count; j++) {
			subseg->levels[j] = gf_bs_read_u8(bs);
			subseg->range_sizes[j] = gf_bs_read_u24(bs);
			ptr->size -= 4;
		}
	}
	return GF_OK;
}
