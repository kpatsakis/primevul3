static int chmd_extract(struct mschm_decompressor *base,
                        struct mschmd_file *file, const char *filename)
{
  struct mschm_decompressor_p *self = (struct mschm_decompressor_p *) base;
  struct mspack_system *sys;
  struct mschmd_header *chm;
  struct mspack_file *fh;
  off_t bytes;

  if (!self) return MSPACK_ERR_ARGS;
  if (!file || !file->section) return self->error = MSPACK_ERR_ARGS;
  sys = self->system;
  chm = file->section->chm;

  /* create decompression state if it doesn't exist */
  if (!self->d) {
    self->d = (struct mschmd_decompress_state *) sys->alloc(sys, sizeof(struct mschmd_decompress_state));
    if (!self->d) return self->error = MSPACK_ERR_NOMEMORY;
    self->d->chm       = chm;
    self->d->offset    = 0;
    self->d->state     = NULL;
    self->d->sys       = *sys;
    self->d->sys.write = &chmd_sys_write;
    self->d->infh      = NULL;
    self->d->outfh     = NULL;
  }

  /* open input chm file if not open, or the open one is a different chm */
  if (!self->d->infh || (self->d->chm != chm)) {
    if (self->d->infh)  sys->close(self->d->infh);
    if (self->d->state) lzxd_free(self->d->state);
    self->d->chm    = chm;
    self->d->offset = 0;
    self->d->state  = NULL;
    self->d->infh   = sys->open(sys, chm->filename, MSPACK_SYS_OPEN_READ);
    if (!self->d->infh) return self->error = MSPACK_ERR_OPEN;
  }

  /* open file for output */
  if (!(fh = sys->open(sys, filename, MSPACK_SYS_OPEN_WRITE))) {
    return self->error = MSPACK_ERR_OPEN;
  }

  /* if file is empty, simply creating it is enough */
  if (!file->length) {
    sys->close(fh);
    return self->error = MSPACK_ERR_OK;
  }

  self->error = MSPACK_ERR_OK;

  switch (file->section->id) {
  case 0: /* Uncompressed section file */
    /* simple seek + copy */
    if (sys->seek(self->d->infh, file->section->chm->sec0.offset
                  + file->offset, MSPACK_SYS_SEEK_START))
    {
      self->error = MSPACK_ERR_SEEK;
    }
    else {
      unsigned char buf[512];
      off_t length = file->length;
      while (length > 0) {
        int run = sizeof(buf);
        if ((off_t)run > length) run = (int)length;
        if (sys->read(self->d->infh, &buf[0], run) != run) {
          self->error = MSPACK_ERR_READ;
          break;
        }
        if (sys->write(fh, &buf[0], run) != run) {
          self->error = MSPACK_ERR_WRITE;
          break;
        }
        length -= run;
      }
    }
    break;

  case 1: /* MSCompressed section file */
    /* (re)initialise compression state if we it is not yet initialised,
     * or we have advanced too far and have to backtrack
     */
    if (!self->d->state || (file->offset < self->d->offset)) {
      if (self->d->state) {
        lzxd_free(self->d->state);
        self->d->state = NULL;
      }
      if (chmd_init_decomp(self, file)) break;
    }

    /* seek to input data */
    if (sys->seek(self->d->infh, self->d->inoffset, MSPACK_SYS_SEEK_START)) {
      self->error = MSPACK_ERR_SEEK;
      break;
    }

    /* get to correct offset. */
    self->d->outfh = NULL;
    if ((bytes = file->offset - self->d->offset)) {
      self->error = lzxd_decompress(self->d->state, bytes);
    }

    /* if getting to the correct offset was error free, unpack file */
    if (!self->error) {
      self->d->outfh = fh;
      self->error = lzxd_decompress(self->d->state, file->length);
    }

    /* save offset in input source stream, in case there is a section 0
     * file between now and the next section 1 file extracted */
    self->d->inoffset = sys->tell(self->d->infh);

    /* if an LZX error occured, the LZX decompressor is now useless */
    if (self->error) {
      if (self->d->state) lzxd_free(self->d->state);
      self->d->state = NULL;
    }
    break;
  }

  sys->close(fh);
  return self->error;
}
