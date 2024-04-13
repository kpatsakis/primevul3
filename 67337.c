mrb_init_io(mrb_state *mrb)
{
  struct RClass *io;

  io      = mrb_define_class(mrb, "IO", mrb->object_class);
  MRB_SET_INSTANCE_TT(io, MRB_TT_DATA);

  mrb_include_module(mrb, io, mrb_module_get(mrb, "Enumerable")); /* 15.2.20.3 */
  mrb_define_class_method(mrb, io, "_popen",  mrb_io_s_popen,   MRB_ARGS_ANY());
  mrb_define_class_method(mrb, io, "_sysclose",  mrb_io_s_sysclose, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, io, "for_fd",  mrb_io_s_for_fd,   MRB_ARGS_ANY());
  mrb_define_class_method(mrb, io, "select",  mrb_io_s_select,  MRB_ARGS_ANY());
  mrb_define_class_method(mrb, io, "sysopen", mrb_io_s_sysopen, MRB_ARGS_ANY());
#ifndef _WIN32
  mrb_define_class_method(mrb, io, "_pipe", mrb_io_s_pipe, MRB_ARGS_NONE());
#endif

  mrb_define_method(mrb, io, "initialize", mrb_io_initialize, MRB_ARGS_ANY());    /* 15.2.20.5.21 (x)*/
  mrb_define_method(mrb, io, "initialize_copy", mrb_io_initialize_copy, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, io, "_check_readable", mrb_io_check_readable, MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "isatty",     mrb_io_isatty,     MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "sync",       mrb_io_sync,       MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "sync=",      mrb_io_set_sync,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, io, "sysread",    mrb_io_sysread,    MRB_ARGS_ANY());
  mrb_define_method(mrb, io, "sysseek",    mrb_io_sysseek,    MRB_ARGS_REQ(1));
  mrb_define_method(mrb, io, "syswrite",   mrb_io_syswrite,   MRB_ARGS_REQ(1));
  mrb_define_method(mrb, io, "close",      mrb_io_close,      MRB_ARGS_NONE());   /* 15.2.20.5.1 */
  mrb_define_method(mrb, io, "close_write",    mrb_io_close_write,       MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "close_on_exec=", mrb_io_set_close_on_exec, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, io, "close_on_exec?", mrb_io_close_on_exec_p,   MRB_ARGS_NONE());
  mrb_define_method(mrb, io, "closed?",    mrb_io_closed,     MRB_ARGS_NONE());   /* 15.2.20.5.2 */
  mrb_define_method(mrb, io, "pid",        mrb_io_pid,        MRB_ARGS_NONE());   /* 15.2.20.5.2 */
  mrb_define_method(mrb, io, "fileno",     mrb_io_fileno,     MRB_ARGS_NONE());


  mrb_gv_set(mrb, mrb_intern_cstr(mrb, "$/"), mrb_str_new_cstr(mrb, "\n"));
}
