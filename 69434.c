gstd_read(void *the_tok, char *buf, int length)
{
	struct gstd_tok	*tok = the_tok;
	gss_buffer_desc	in;
	OM_uint32	maj, min;
	int		bufpos = tok->gstd_inbufpos;
	int		ret;

	/*
	 * If we have no buffered data, read another packet and
	 * reset the buffer.
	 */

	if (bufpos == -1 || bufpos >= tok->gstd_inbuf.length) {
		if (tok->gstd_inbuf.length > 0)
			gss_release_buffer(&min, &tok->gstd_inbuf);

		/*
		 * If we encounter a protocol botch or if the other side has
		 * closed the connection, we return that fact here
		 */
		ret = read_packet(tok->gstd_fd, &in, 0, 0);
		if (ret <= 0)
			return ret;

		maj = gss_unwrap(&min, tok->gstd_ctx, &in, &tok->gstd_inbuf,
		    NULL, NULL);
		if (maj != GSS_S_COMPLETE) {
			gstd_error(LOG_ERR, min, "gss_unwrap");
			return -1;
		}
		gss_release_buffer(&min, &in);
		bufpos = 0;
	}

	/*
	 * Now we know that we have a buffered packet, so return
	 * as much of it as we can.  We do not need to fill the
	 * requestor's buffer, because stdio can deal with short
	 * reads.
	 */

	length = MIN(length, tok->gstd_inbuf.length - bufpos);
	memcpy(buf, tok->gstd_inbuf.value + bufpos, length);
	tok->gstd_inbufpos = bufpos + length;
	LOG(LOG_DEBUG, ("read %d bytes", length));
	return length;
}
