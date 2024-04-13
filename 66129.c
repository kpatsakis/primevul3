GF_Box *tfhd_New()
{
	ISOM_DECL_BOX_ALLOC(GF_TrackFragmentHeaderBox, GF_ISOM_BOX_TYPE_TFHD);
	return (GF_Box *)tmp;
}
