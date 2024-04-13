static int jas_iccgetxyz(jas_stream_t *in, jas_iccxyz_t *xyz)
{
	if (jas_iccgetsint32(in, &xyz->x) ||
	  jas_iccgetsint32(in, &xyz->y) ||
	  jas_iccgetsint32(in, &xyz->z)) {
		return -1;
	}
	return 0;
}
