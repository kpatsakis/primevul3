mrb_io_initialize(mrb_state *mrb, mrb_value io)
{
  struct mrb_io *fptr;
  mrb_int fd;
  mrb_value mode, opt;
  int flags;

  mode = opt = mrb_nil_value();

  mrb_get_args(mrb, "i|So", &fd, &mode, &opt);
  if (mrb_nil_p(mode)) {
    mode = mrb_str_new_cstr(mrb, "r");
  }
  if (mrb_nil_p(opt)) {
    opt = mrb_hash_new(mrb);
  }

  flags = mrb_io_modestr_to_flags(mrb, mrb_string_value_cstr(mrb, &mode));

  mrb_iv_set(mrb, io, mrb_intern_cstr(mrb, "@buf"), mrb_str_new_cstr(mrb, ""));

  fptr = (struct mrb_io *)DATA_PTR(io);
  if (fptr != NULL) {
    fptr_finalize(mrb, fptr, TRUE);
    mrb_free(mrb, fptr);
  }
  fptr = mrb_io_alloc(mrb);

  DATA_TYPE(io) = &mrb_io_type;
  DATA_PTR(io) = fptr;

  fptr->fd = (int)fd;
  fptr->readable = ((flags & FMODE_READABLE) != 0);
  fptr->writable = ((flags & FMODE_WRITABLE) != 0);
  fptr->sync = 0;
  return io;
}
