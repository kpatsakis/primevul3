gdImagePtr gdImageCreateFromWebpCtx (gdIOCtx * infile)
{
	int    width, height;
	uint8_t   *filedata = NULL;
	uint8_t    *argb = NULL;
	size_t size = 0, n;
	gdImagePtr im;
	int x, y;
	uint8_t *p;

	do {
		unsigned char *read, *temp;

		temp = gdRealloc(filedata, size+GD_WEBP_ALLOC_STEP);
		if (temp) {
			filedata = temp;
			read = temp + size;
		} else {
			if (filedata) {
				gdFree(filedata);
			}
			zend_error(E_ERROR, "WebP decode: realloc failed");
			return NULL;
		}

		n = gdGetBuf(read, GD_WEBP_ALLOC_STEP, infile);
		if (n>0 && n!=EOF) {
			size += n;
		}
	} while (n>0 && n!=EOF);

	if (WebPGetInfo(filedata,size, &width, &height) == 0) {
		zend_error(E_ERROR, "gd-webp cannot get webp info");
		gdFree(filedata);
		return NULL;
	}

	im = gdImageCreateTrueColor(width, height);
	if (!im) {
		gdFree(filedata);
		return NULL;
	}
	argb = WebPDecodeARGB(filedata, size, &width, &height);
	if (!argb) {
		zend_error(E_ERROR, "gd-webp cannot allocate temporary buffer");
		gdFree(filedata);
		gdImageDestroy(im);
		return NULL;
	}
	for (y = 0, p = argb;  y < height; y++) {
		for (x = 0; x < width; x++) {
			register uint8_t a = gdAlphaMax - (*(p++) >> 1);
			register uint8_t r = *(p++);
			register uint8_t g = *(p++);
			register uint8_t b = *(p++);
			im->tpixels[y][x] = gdTrueColorAlpha(r, g, b, a);
		}
	}
	gdFree(filedata);
	/* do not use gdFree here, in case gdFree/alloc is mapped to something else than libc */
	free(argb);
	im->saveAlphaFlag = 1;
	return im;
}
