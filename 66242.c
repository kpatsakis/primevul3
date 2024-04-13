GF_Box *unkn_New(u32 box_type)
{
	ISOM_DECL_BOX_ALLOC(GF_UnknownBox, GF_ISOM_BOX_TYPE_UNKNOWN);
	tmp->original_4cc = box_type;
	return (GF_Box *) tmp;
}
