static int file_read(jas_stream_obj_t *obj, char *buf, int cnt)
{
	jas_stream_fileobj_t *fileobj;
	JAS_DBGLOG(100, ("file_read(%p, %p, %d)\n", obj, buf, cnt));
	fileobj = JAS_CAST(jas_stream_fileobj_t *, obj);
	return read(fileobj->fd, buf, cnt);
}
