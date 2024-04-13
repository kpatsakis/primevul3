static int kwajd_decompress(struct mskwaj_decompressor *base,
			    const char *input, const char *output)
{
    struct mskwaj_decompressor_p *self = (struct mskwaj_decompressor_p *) base;
    struct mskwajd_header *hdr;
    int error;

    if (!self) return MSPACK_ERR_ARGS;

    if (!(hdr = kwajd_open(base, input))) return self->error;
    error = kwajd_extract(base, hdr, output);
    kwajd_close(base, hdr);
    return self->error = error;
}
