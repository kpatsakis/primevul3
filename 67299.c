void fprintf_ngiflib_gif(FILE * f, struct ngiflib_gif * g) {
	struct ngiflib_img * i;
	fprintf(f, "* ngiflib_gif @ %p %s\n", g, g->signature);
	fprintf(f, "  %dx%d, %d bits, %d couleurs\n", g->width, g->height, g->imgbits, g->ncolors);
	fprintf(f, "  palette = %p, backgroundcolorindex %d\n", g->palette, g->backgroundindex);
	fprintf(f, "  pixelaspectratio = %d\n", g->pixaspectratio);
	fprintf(f, "  frbuff = %p\n", g->frbuff.p8);
	
	fprintf(f, "  cur_img = %p\n", g->cur_img);
	fprintf(f, "  %d images :\n", g->nimg);
	i = g->first_img;
	while(i) {
		fprintf_ngiflib_img(f, i);
		i = i->next;
	}
}
