BGD_DECLARE(gdImagePtr) gdImageCreateFromTiff(FILE *inFile)
{
	gdImagePtr im;
	gdIOCtx *in = gdNewFileCtx(inFile);
	if (in == NULL) return NULL;
	im = gdImageCreateFromTiffCtx(in);
	in->gd_free(in);
	return im;
}
