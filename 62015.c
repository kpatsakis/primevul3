static int pnm_gethdr(jas_stream_t *in, pnm_hdr_t *hdr)
{
	int_fast32_t maxval;
	int_fast32_t width;
	int_fast32_t height;
	int type;

	if (pnm_getint16(in, &hdr->magic) || pnm_getsintstr(in, &width) ||
	  pnm_getsintstr(in, &height)) {
		return -1;
	}
	hdr->width = width;
	hdr->height = height;
	if ((type = pnm_type(hdr->magic)) == PNM_TYPE_INVALID) {
		return -1;
	}
	if (type != PNM_TYPE_PBM) {
		if (pnm_getsintstr(in, &maxval)) {
			return -1;
		}
	} else {
		maxval = 1;
	}
	if (maxval < 0) {
		hdr->maxval = -maxval;
		hdr->sgnd = true;
	} else {
		hdr->maxval = maxval;
		hdr->sgnd = false;
	}

	switch (type) {
	case PNM_TYPE_PBM:
	case PNM_TYPE_PGM:
		hdr->numcmpts = 1;
		break;
	case PNM_TYPE_PPM:
		hdr->numcmpts = 3;
		break;
	default:
		abort();
		break;
	}

	return 0;
}
