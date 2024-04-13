static int ras_gethdr(jas_stream_t *in, ras_hdr_t *hdr)
{
	if (ras_getint(in, &hdr->magic) || ras_getint(in, &hdr->width) ||
	  ras_getint(in, &hdr->height) || ras_getint(in, &hdr->depth) ||
	  ras_getint(in, &hdr->length) || ras_getint(in, &hdr->type) ||
	  ras_getint(in, &hdr->maptype) || ras_getint(in, &hdr->maplength)) {
		return -1;
	}

	if (hdr->magic != RAS_MAGIC) {
		return -1;
	}

	return 0;
}
