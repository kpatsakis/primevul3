GF_Box *sdtp_New()
{
	ISOM_DECL_BOX_ALLOC(GF_SampleDependencyTypeBox, GF_ISOM_BOX_TYPE_SDTP);
	tmp->flags = 1;
	return (GF_Box *)tmp;
}
