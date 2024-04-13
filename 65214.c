static int lzh_decompress(struct kwajd_stream *lzh)
{
    register unsigned int bit_buffer;
    register int bits_left, i;
    register unsigned short sym;
    unsigned char *i_ptr, *i_end, lit_run = 0;
    int j, pos = 0, len, offset, err;
    unsigned int types[6];

    /* reset global state */
    INIT_BITS;
    RESTORE_BITS;
    memset(&lzh->window[0], LZSS_WINDOW_FILL, (size_t) LZSS_WINDOW_SIZE);

    /* read 6 encoding types (for byte alignment) but only 5 are needed */
    for (i = 0; i < 6; i++) READ_BITS_SAFE(types[i], 4);

    /* read huffman table symbol lengths and build huffman trees */
    BUILD_TREE(MATCHLEN1, types[0]);
    BUILD_TREE(MATCHLEN2, types[1]);
    BUILD_TREE(LITLEN,    types[2]);
    BUILD_TREE(OFFSET,    types[3]);
    BUILD_TREE(LITERAL,   types[4]);

    while (!lzh->input_end) {
	if (lit_run) READ_HUFFSYM_SAFE(MATCHLEN2, len);
	else         READ_HUFFSYM_SAFE(MATCHLEN1, len);

	if (len > 0) {
	    len += 2;
	    lit_run = 0; /* not the end of a literal run */
	    READ_HUFFSYM_SAFE(OFFSET, j); offset = j << 6;
	    READ_BITS_SAFE(j, 6);         offset |= j;

	    /* copy match as output and into the ring buffer */
	    while (len-- > 0) {
		lzh->window[pos] = lzh->window[(pos+4096-offset) & 4095];
		WRITE_BYTE;
		pos++; pos &= 4095;
	    }
	}
	else {
	    READ_HUFFSYM_SAFE(LITLEN, len); len++;
	    lit_run = (len == 32) ? 0 : 1; /* end of a literal run? */
	    while (len-- > 0) {
		READ_HUFFSYM_SAFE(LITERAL, j);
		/* copy as output and into the ring buffer */
		lzh->window[pos] = j;
		WRITE_BYTE;
		pos++; pos &= 4095;
	    }
	}
    }
    return MSPACK_ERR_OK;
}
