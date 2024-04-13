void fprintf_ngiflib_img(FILE * f, struct ngiflib_img * i) {
	fprintf(f, "  * ngiflib_img @ %p\n", i);
	fprintf(f, "    next = %p\n", i->next);
	fprintf(f, "    parent = %p\n", i->parent);
	fprintf(f, "    palette = %p\n", i->palette);
	fprintf(f, "    %3d couleurs", i->ncolors);
	if(i->interlaced) fprintf(f, " interlaced");
	fprintf(f, "\n    taille : %dx%d, pos (%d,%d)\n", i->width, i->height, i->posX, i->posY);
	fprintf(f, "    sort_flag=%x localpalbits=%d\n", i->sort_flag, i->localpalbits);
}
