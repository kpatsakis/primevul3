  mspack_create_chm_decompressor(struct mspack_system *sys)
{
  struct mschm_decompressor_p *self = NULL;

  if (!sys) sys = mspack_default_system;
  if (!mspack_valid_system(sys)) return NULL;

  if ((self = (struct mschm_decompressor_p *) sys->alloc(sys, sizeof(struct mschm_decompressor_p)))) {
    self->base.open       = &chmd_open;
    self->base.close      = &chmd_close;
    self->base.extract    = &chmd_extract;
    self->base.last_error = &chmd_error;
    self->base.fast_open  = &chmd_fast_open;
    self->base.fast_find  = &chmd_fast_find;
    self->system          = sys;
    self->error           = MSPACK_ERR_OK;
    self->d               = NULL;
  }
  return (struct mschm_decompressor *) self;
}
