io_set_process_status(mrb_state *mrb, pid_t pid, int status)
{
  struct RClass *c_process, *c_status;
  mrb_value v;

  c_status = NULL;
  if (mrb_class_defined(mrb, "Process")) {
    c_process = mrb_module_get(mrb, "Process");
    if (mrb_const_defined(mrb, mrb_obj_value(c_process), mrb_intern_cstr(mrb, "Status"))) {
      c_status = mrb_class_get_under(mrb, c_process, "Status");
    }
  }
  if (c_status != NULL) {
    v = mrb_funcall(mrb, mrb_obj_value(c_status), "new", 2, mrb_fixnum_value(pid), mrb_fixnum_value(status));
  } else {
    v = mrb_fixnum_value(WEXITSTATUS(status));
  }
  mrb_gv_set(mrb, mrb_intern_cstr(mrb, "$?"), v);
}
