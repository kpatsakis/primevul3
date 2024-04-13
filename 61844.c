static int jas_iccxyz_input(jas_iccattrval_t *attrval, jas_stream_t *in,
  int len)
{
	if (len != 4 * 3) abort();
	return jas_iccgetxyz(in, &attrval->data.xyz);
}
