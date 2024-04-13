void mspack_destroy_kwaj_decompressor(struct mskwaj_decompressor *base)
{
    struct mskwaj_decompressor_p *self = (struct mskwaj_decompressor_p *) base;
    if (self) {
	struct mspack_system *sys = self->system;
	sys->free(self);
    }
}
