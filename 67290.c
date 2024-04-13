static void FillGifBackGround(struct ngiflib_gif * g) {
	long n = (long)g->width*g->height;
#ifndef NGIFLIB_INDEXED_ONLY
	u32 bg_truecolor;
#endif /* NGIFLIB_INDEXED_ONLY */

	if((g->frbuff.p8==NULL)||(g->palette==NULL)) return;
#ifndef NGIFLIB_INDEXED_ONLY
	if(g->mode & NGIFLIB_MODE_INDEXED) {
#endif /* NGIFLIB_INDEXED_ONLY */
		ngiflib_memset(g->frbuff.p8, g->backgroundindex, n);
#ifndef NGIFLIB_INDEXED_ONLY
	} else {
		u32 * p = g->frbuff.p32;
		bg_truecolor = GifIndexToTrueColor(g->palette, g->backgroundindex);
		while(n-->0) *p++ = bg_truecolor;
	}
#endif /* NGIFLIB_INDEXED_ONLY */
}
