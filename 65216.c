static int lzh_read_input(struct kwajd_stream *lzh) {
    int read;
    if (lzh->input_end) {
	lzh->input_end += 8;
	lzh->inbuf[0] = 0;
	read = 1;
    }
    else {
	read = lzh->sys->read(lzh->input, &lzh->inbuf[0], KWAJ_INPUT_SIZE);
	if (read < 0) return MSPACK_ERR_READ;
	if (read == 0) {
	    lzh->input_end = 8;
	    lzh->inbuf[0] = 0;
	    read = 1;
	}
    }

    /* update i_ptr and i_end */
    lzh->i_ptr = &lzh->inbuf[0];
    lzh->i_end = &lzh->inbuf[read];
    return MSPACK_ERR_OK;
}
