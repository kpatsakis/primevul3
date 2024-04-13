tiff_handle * new_tiff_handle(gdIOCtx *g)
{
	tiff_handle * t;

	if (!g) {
		gd_error("Cannot create a new tiff handle, missing Ctx argument");
		return NULL;
	}

	t = (tiff_handle *) gdMalloc(sizeof(tiff_handle));
	if (!t) {
		gd_error("Failed to allocate a new tiff handle");
		return NULL;
	}

	t->size = 0;
	t->pos = 0;
	t->ctx = g;
	t->written = 0;

	return t;
}
