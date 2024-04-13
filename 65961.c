GF_Box *pdin_New()
{
	ISOM_DECL_BOX_ALLOC(GF_ProgressiveDownloadBox, GF_ISOM_BOX_TYPE_PDIN);
	tmp->flags = 1;
	return (GF_Box *)tmp;
}
