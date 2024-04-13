static int chmd_error(struct mschm_decompressor *base) {
  struct mschm_decompressor_p *self = (struct mschm_decompressor_p *) base;
  return (self) ? self->error : MSPACK_ERR_ARGS;
}
