    mspack_create_kwaj_decompressor(struct mspack_system *sys)
{
  struct mskwaj_decompressor_p *self = NULL;

  if (!sys) sys = mspack_default_system;
  if (!mspack_valid_system(sys)) return NULL;

  if ((self = (struct mskwaj_decompressor_p *) sys->alloc(sys, sizeof(struct mskwaj_decompressor_p)))) {
    self->base.open       = &kwajd_open;
    self->base.close      = &kwajd_close;
    self->base.extract    = &kwajd_extract;
    self->base.decompress = &kwajd_decompress;
    self->base.last_error = &kwajd_error;
    self->system          = sys;
    self->error           = MSPACK_ERR_OK;
  }
  return (struct mskwaj_decompressor *) self;
}
