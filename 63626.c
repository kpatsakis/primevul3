void mspack_destroy_chm_decompressor(struct mschm_decompressor *base) {
  struct mschm_decompressor_p *self = (struct mschm_decompressor_p *) base;
  if (self) {
    struct mspack_system *sys = self->system;
    if (self->d) {
      if (self->d->infh)  sys->close(self->d->infh);
      if (self->d->state) lzxd_free(self->d->state);
      sys->free(self->d);
    }
    sys->free(self);
  }
}
