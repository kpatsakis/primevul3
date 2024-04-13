static int kwajd_extract(struct mskwaj_decompressor *base,
			 struct mskwajd_header *hdr, const char *filename)
{
    struct mskwaj_decompressor_p *self = (struct mskwaj_decompressor_p *) base;
    struct mspack_system *sys;
    struct mspack_file *fh, *outfh;

    if (!self) return MSPACK_ERR_ARGS;
    if (!hdr) return self->error = MSPACK_ERR_ARGS;

    sys = self->system;
    fh = ((struct mskwajd_header_p *) hdr)->fh;

    /* seek to the compressed data */
    if (sys->seek(fh, hdr->data_offset, MSPACK_SYS_SEEK_START)) {
	return self->error = MSPACK_ERR_SEEK;
    }

    /* open file for output */
    if (!(outfh = sys->open(sys, filename, MSPACK_SYS_OPEN_WRITE))) {
	return self->error = MSPACK_ERR_OPEN;
    }

    self->error = MSPACK_ERR_OK;

    /* decompress based on format */
    if (hdr->comp_type == MSKWAJ_COMP_NONE ||
	hdr->comp_type == MSKWAJ_COMP_XOR)
    {
	/* NONE is a straight copy. XOR is a copy xored with 0xFF */
	unsigned char *buf = (unsigned char *) sys->alloc(sys, (size_t) KWAJ_INPUT_SIZE);
	if (buf) {
	    int read, i;
	    while ((read = sys->read(fh, buf, KWAJ_INPUT_SIZE)) > 0) {
		if (hdr->comp_type == MSKWAJ_COMP_XOR) {
		    for (i = 0; i < read; i++) buf[i] ^= 0xFF;
		}
		if (sys->write(outfh, buf, read) != read) {
		    self->error = MSPACK_ERR_WRITE;
		    break;
		}
	    }
	    if (read < 0) self->error = MSPACK_ERR_READ;
	    sys->free(buf);
	}
	else {
	    self->error = MSPACK_ERR_NOMEMORY;
	}
    }
    else if (hdr->comp_type == MSKWAJ_COMP_SZDD) {
	self->error = lzss_decompress(sys, fh, outfh, KWAJ_INPUT_SIZE,
				      LZSS_MODE_EXPAND);
    }
    else if (hdr->comp_type == MSKWAJ_COMP_LZH) {
	struct kwajd_stream *lzh = lzh_init(sys, fh, outfh);
	self->error = (lzh) ? lzh_decompress(lzh) : MSPACK_ERR_NOMEMORY;
	lzh_free(lzh);
    }
    else if (hdr->comp_type == MSKWAJ_COMP_MSZIP) {
        struct mszipd_stream *zip = mszipd_init(sys,fh,outfh,KWAJ_INPUT_SIZE,0);
        self->error = (zip) ? mszipd_decompress_kwaj(zip) : MSPACK_ERR_NOMEMORY;
        mszipd_free(zip);
    }
    else {
	self->error = MSPACK_ERR_DATAFORMAT;
    }

    /* close output file */
    sys->close(outfh);

    return self->error;
}
