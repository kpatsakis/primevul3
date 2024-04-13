void ssix_del(GF_Box *s)
{
	u32 i;
	GF_SubsegmentIndexBox *ptr = (GF_SubsegmentIndexBox *)s;
	if (ptr == NULL) return;
	if (ptr->subsegments) {
		for (i = 0; i < ptr->subsegment_count; i++) {
			GF_Subsegment *subsegment = &ptr->subsegments[i];
			if (subsegment->levels) gf_free(subsegment->levels);
			if (subsegment->range_sizes) gf_free(subsegment->range_sizes);
		}
		gf_free(ptr->subsegments);
	}
	gf_free(ptr);
}
