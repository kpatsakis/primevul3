static int jas_iccxyz_output(jas_iccattrval_t *attrval, jas_stream_t *out)
{
	jas_iccxyz_t *xyz = &attrval->data.xyz;
	if (jas_iccputuint32(out, xyz->x) ||
	  jas_iccputuint32(out, xyz->y) ||
	  jas_iccputuint32(out, xyz->z))
		return -1;
	return 0;
}
