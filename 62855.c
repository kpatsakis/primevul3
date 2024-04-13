_warc_rdhdr(struct archive_read *a, struct archive_entry *entry)
{
#define HDR_PROBE_LEN		(12U)
	struct warc_s *w = a->format->data;
	unsigned int ver;
	const char *buf;
	ssize_t nrd;
	const char *eoh;
	/* for the file name, saves some strndup()'ing */
	warc_string_t fnam;
	/* warc record type, not that we really use it a lot */
	warc_type_t ftyp;
	/* content-length+error monad */
	ssize_t cntlen;
	/* record time is the WARC-Date time we reinterpret it as ctime */
	time_t rtime;
	/* mtime is the Last-Modified time which will be the entry's mtime */
	time_t mtime;

start_over:
	/* just use read_ahead() they keep track of unconsumed
	 * bits and bobs for us; no need to put an extra shift in
	 * and reproduce that functionality here */
	buf = __archive_read_ahead(a, HDR_PROBE_LEN, &nrd);

	if (nrd < 0) {
		/* no good */
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Bad record header");
		return (ARCHIVE_FATAL);
	} else if (buf == NULL) {
		/* there should be room for at least WARC/bla\r\n
		 * must be EOF therefore */
		return (ARCHIVE_EOF);
	}
 	/* looks good so far, try and find the end of the header now */
	eoh = _warc_find_eoh(buf, nrd);
	if (eoh == NULL) {
		/* still no good, the header end might be beyond the
		 * probe we've requested, but then again who'd cram
		 * so much stuff into the header *and* be 28500-compliant */
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Bad record header");
		return (ARCHIVE_FATAL);
	}
	ver = _warc_rdver(buf, eoh - buf);
	/* we currently support WARC 0.12 to 1.0 */
	if (ver == 0U) {
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Invalid record version");
		return (ARCHIVE_FATAL);
	} else if (ver < 1200U || ver > 10000U) {
		archive_set_error(
			&a->archive, ARCHIVE_ERRNO_MISC,
			"Unsupported record version: %u.%u",
			ver / 10000, (ver % 10000) / 100);
		return (ARCHIVE_FATAL);
	}
	cntlen = _warc_rdlen(buf, eoh - buf);
	if (cntlen < 0) {
		/* nightmare!  the specs say content-length is mandatory
		 * so I don't feel overly bad stopping the reader here */
		archive_set_error(
			&a->archive, EINVAL,
			"Bad content length");
		return (ARCHIVE_FATAL);
	}
	rtime = _warc_rdrtm(buf, eoh - buf);
	if (rtime == (time_t)-1) {
		/* record time is mandatory as per WARC/1.0,
		 * so just barf here, fast and loud */
		archive_set_error(
			&a->archive, EINVAL,
			"Bad record time");
		return (ARCHIVE_FATAL);
	}

	/* let the world know we're a WARC archive */
	a->archive.archive_format = ARCHIVE_FORMAT_WARC;
	if (ver != w->pver) {
		/* stringify this entry's version */
		archive_string_sprintf(&w->sver,
			"WARC/%u.%u", ver / 10000, (ver % 10000) / 100);
		/* remember the version */
		w->pver = ver;
	}
	/* start off with the type */
	ftyp = _warc_rdtyp(buf, eoh - buf);
	/* and let future calls know about the content */
	w->cntlen = cntlen;
	w->cntoff = 0U;
	mtime = 0;/* Avoid compiling error on some platform. */

	switch (ftyp) {
	case WT_RSRC:
	case WT_RSP:
		/* only try and read the filename in the cases that are
		 * guaranteed to have one */
		fnam = _warc_rduri(buf, eoh - buf);
		/* check the last character in the URI to avoid creating
		 * directory endpoints as files, see Todo above */
		if (fnam.len == 0 || fnam.str[fnam.len - 1] == '/') {
			/* break here for now */
			fnam.len = 0U;
			fnam.str = NULL;
			break;
		}
		/* bang to our string pool, so we save a
		 * malloc()+free() roundtrip */
		if (fnam.len + 1U > w->pool.len) {
			w->pool.len = ((fnam.len + 64U) / 64U) * 64U;
			w->pool.str = realloc(w->pool.str, w->pool.len);
		}
		memcpy(w->pool.str, fnam.str, fnam.len);
		w->pool.str[fnam.len] = '\0';
		/* let no one else know about the pool, it's a secret, shhh */
		fnam.str = w->pool.str;

		/* snarf mtime or deduce from rtime
		 * this is a custom header added by our writer, it's quite
		 * hard to believe anyone else would go through with it
		 * (apart from being part of some http responses of course) */
		if ((mtime = _warc_rdmtm(buf, eoh - buf)) == (time_t)-1) {
			mtime = rtime;
		}
		break;
	default:
		fnam.len = 0U;
		fnam.str = NULL;
		break;
	}

	/* now eat some of those delicious buffer bits */
	__archive_read_consume(a, eoh - buf);

	switch (ftyp) {
	case WT_RSRC:
	case WT_RSP:
		if (fnam.len > 0U) {
			/* populate entry object */
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_copy_pathname(entry, fnam.str);
			archive_entry_set_size(entry, cntlen);
			archive_entry_set_perm(entry, 0644);
			/* rtime is the new ctime, mtime stays mtime */
			archive_entry_set_ctime(entry, rtime, 0L);
			archive_entry_set_mtime(entry, mtime, 0L);
			break;
		}
		/* FALLTHROUGH */
	default:
		/* consume the content and start over */
		_warc_skip(a);
		goto start_over;
	}
	return (ARCHIVE_OK);
}
