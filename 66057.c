GF_Box *stbl_New()
{
	ISOM_DECL_BOX_ALLOC(GF_SampleTableBox, GF_ISOM_BOX_TYPE_STBL);
	tmp->MaxSamplePerChunk = 10;
	tmp->groupID = 1;
	return (GF_Box *)tmp;
}
