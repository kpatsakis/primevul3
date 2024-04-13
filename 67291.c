static u8 GetByte(struct ngiflib_gif * g) {
#ifndef NGIFLIB_NO_FILE
	if(g->mode & NGIFLIB_MODE_FROM_MEM) {
#endif /* NGIFLIB_NO_FILE */
		return *(g->input.bytes++);
#ifndef NGIFLIB_NO_FILE
	} else {
		return (u8)(getc(g->input.file));
	}
#endif /* NGIFLIB_NO_FILE */
}
