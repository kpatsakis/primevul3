void GifDestroy(struct ngiflib_gif * g) {
	if(g==NULL) return;
	if(g->palette) ngiflib_free(g->palette);
	if(g->frbuff.p8) ngiflib_free(g->frbuff.p8);
	GifImgDestroy(g->first_img);
	ngiflib_free(g);
}
