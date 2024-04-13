static toff_t tiff_seekproc(thandle_t clientdata, toff_t offset, int from)
{
	tiff_handle *th = (tiff_handle *)clientdata;
	gdIOCtx *ctx = th->ctx;
	int result;

	switch(from) {
	default:
	case SEEK_SET:
		/* just use offset */
		break;

	case SEEK_END:
		/* invert offset, so that it is from start, not end as supplied */
		offset = th->size + offset;
		break;

	case SEEK_CUR:
		/* add current position to translate it to 'from start',
		 * not from durrent as supplied
		 */
		offset += th->pos;
		break;
	}

	/* now, move pos in both io context and buf */
	if((result = (ctx->seek)(ctx, offset))) {
		th->pos = offset;
	}

	return result ? offset : (toff_t)-1;
}
