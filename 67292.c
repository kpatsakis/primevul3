static int GetByteStr(struct ngiflib_gif * g, u8 * p, int n) {
	if(!p) return -1;
#ifndef NGIFLIB_NO_FILE
	if(g->mode & NGIFLIB_MODE_FROM_MEM) {
#endif /* NGIFLIB_NO_FILE */
		ngiflib_memcpy(p, g->input.bytes, n);
		g->input.bytes += n;
		return 0;
#ifndef NGIFLIB_NO_FILE
	} else {
		size_t read;
		read = fread(p, 1, n, g->input.file);
		return ((int)read == n) ? 0 : -1;
	}
#endif /* NGIFLIB_NO_FILE */
}
