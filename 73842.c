void gdImageWebp (gdImagePtr im, FILE * outFile)
{
	gdIOCtx *out = gdNewFileCtx(outFile);
	gdImageWebpCtx(im, out, -1);
	out->gd_free(out);
}
