static int jas_iccputxyz(jas_stream_t *out, jas_iccxyz_t *xyz)
{
	jas_iccputuint32(out, xyz->x);
	jas_iccputuint32(out, xyz->y);
	jas_iccputuint32(out, xyz->z);
	return 0;
}
