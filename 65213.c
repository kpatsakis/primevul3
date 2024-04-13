static struct mskwajd_header *kwajd_open(struct mskwaj_decompressor *base,
					 const char *filename)
{
    struct mskwaj_decompressor_p *self = (struct mskwaj_decompressor_p *) base;
    struct mskwajd_header *hdr;
    struct mspack_system *sys;
    struct mspack_file *fh;

    if (!self) return NULL;
    sys = self->system;

    fh  = sys->open(sys, filename, MSPACK_SYS_OPEN_READ);
    hdr = (struct mskwajd_header *) sys->alloc(sys, sizeof(struct mskwajd_header_p));
    if (fh && hdr) {
	((struct mskwajd_header_p *) hdr)->fh = fh;
	self->error = kwajd_read_headers(sys, fh, hdr);
    }
    else {
	if (!fh)  self->error = MSPACK_ERR_OPEN;
	if (!hdr) self->error = MSPACK_ERR_NOMEMORY;
    }
    
    if (self->error) {
	if (fh)  sys->close(fh);
	if (hdr) sys->free(hdr);
	hdr = NULL;
    }

    return hdr;
}
