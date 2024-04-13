void GifImgDestroy(struct ngiflib_img * i) {
	if(i==NULL) return;
	if(i->next) GifImgDestroy(i->next);
	if(i->palette && (i->palette != i->parent->palette))
	  ngiflib_free(i->palette);
	ngiflib_free(i);
}
