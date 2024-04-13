static int pgx_getdata(jas_stream_t *in, pgx_hdr_t *hdr, jas_image_t *image)
{
	jas_matrix_t *data;
	uint_fast32_t x;
	uint_fast32_t y;
	uint_fast32_t word;
	int_fast32_t v;

	data = 0;

	if (!(data = jas_matrix_create(1, hdr->width))) {
		goto error;
	}
	for (y = 0; y < hdr->height; ++y) {
		for (x = 0; x < hdr->width; ++x) {
			/* Need to adjust signed value. */
			if ((v = pgx_getword(in, hdr->bigendian, hdr->prec)) < 0) {
				goto error;
			}
			word = v;
			v = pgx_wordtoint(word, hdr->prec, hdr->sgnd);
			jas_matrix_set(data, 0, x, v);
		}
		if (jas_image_writecmpt(image, 0, 0, y, hdr->width, 1, data)) {
			goto error;
		}
	}
	jas_matrix_destroy(data);
	return 0;

error:
	if (data) {
		jas_matrix_destroy(data);
	}
	return -1;
}
