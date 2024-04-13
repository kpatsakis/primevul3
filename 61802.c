static int jas_icccurv_output(jas_iccattrval_t *attrval, jas_stream_t *out)
{
	jas_icccurv_t *curv = &attrval->data.curv;
	unsigned int i;

	if (jas_iccputuint32(out, curv->numents))
		goto error;
	for (i = 0; i < curv->numents; ++i) {
		if (jas_iccputuint16(out, curv->ents[i]))
			goto error;
	}
	return 0;
error:
	return -1;
}
