mrb_io_s_pipe(mrb_state *mrb, mrb_value klass)
{
  mrb_value r = mrb_nil_value();
  mrb_value w = mrb_nil_value();
  struct mrb_io *fptr_r;
  struct mrb_io *fptr_w;
  int pipes[2];

  if (mrb_pipe(mrb, pipes) == -1) {
    mrb_sys_fail(mrb, "pipe");
  }

  r = mrb_obj_value(mrb_data_object_alloc(mrb, mrb_class_ptr(klass), NULL, &mrb_io_type));
  mrb_iv_set(mrb, r, mrb_intern_cstr(mrb, "@buf"), mrb_str_new_cstr(mrb, ""));
  fptr_r = mrb_io_alloc(mrb);
  fptr_r->fd = pipes[0];
  fptr_r->readable = 1;
  fptr_r->writable = 0;
  fptr_r->sync = 0;
  DATA_TYPE(r) = &mrb_io_type;
  DATA_PTR(r)  = fptr_r;

  w = mrb_obj_value(mrb_data_object_alloc(mrb, mrb_class_ptr(klass), NULL, &mrb_io_type));
  mrb_iv_set(mrb, w, mrb_intern_cstr(mrb, "@buf"), mrb_str_new_cstr(mrb, ""));
  fptr_w = mrb_io_alloc(mrb);
  fptr_w->fd = pipes[1];
  fptr_w->readable = 0;
  fptr_w->writable = 1;
  fptr_w->sync = 1;
  DATA_TYPE(w) = &mrb_io_type;
  DATA_PTR(w)  = fptr_w;

  return mrb_assoc_new(mrb, r, w);
}
