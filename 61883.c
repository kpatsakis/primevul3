static long sfile_seek(jas_stream_obj_t *obj, long offset, int origin)
{
	FILE *fp;
	JAS_DBGLOG(100, ("sfile_seek(%p, %ld, %d)\n", obj, offset, origin));
	fp = JAS_CAST(FILE *, obj);
	return fseek(fp, offset, origin);
}
