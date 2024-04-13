static mif_hdr_t *mif_hdr_get(jas_stream_t *in)
{
	uchar magicbuf[MIF_MAGICLEN];
	char buf[4096];
	mif_hdr_t *hdr;
	bool done;
	jas_tvparser_t *tvp;
	int id;

	hdr = 0;

	if (jas_stream_read(in, magicbuf, MIF_MAGICLEN) != MIF_MAGICLEN) {
		goto error;
	}
	if (magicbuf[0] != (MIF_MAGIC >> 24) || magicbuf[1] != ((MIF_MAGIC >> 16) &
	  0xff) || magicbuf[2] != ((MIF_MAGIC >> 8) & 0xff) || magicbuf[3] !=
	  (MIF_MAGIC & 0xff)) {
		jas_eprintf("error: bad signature\n");
		goto error;
	}

	if (!(hdr = mif_hdr_create(0))) {
		goto error;
	}

	done = false;
	do {
		if (!mif_getline(in, buf, sizeof(buf))) {
			goto error;
		}
		if (buf[0] == '\0') {
			continue;
		}
		if (!(tvp = jas_tvparser_create(buf))) {
			goto error;
		}
		if (jas_tvparser_next(tvp)) {
			abort();
		}
		id = jas_taginfo_nonull(jas_taginfos_lookup(mif_tags2, jas_tvparser_gettag(tvp)))->id;
		jas_tvparser_destroy(tvp);
		switch (id) {
		case MIF_CMPT:
			mif_process_cmpt(hdr, buf);
			break;
		case MIF_END:
			done = 1;
			break;
		}
	} while (!done);

	return hdr;

error:
	if (hdr) {
		mif_hdr_destroy(hdr);
	}
	return 0;
}
