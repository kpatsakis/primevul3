mrb_pipe(mrb_state *mrb, int pipes[2])
{
  int ret;
  ret = mrb_cloexec_pipe(mrb, pipes);
  if (ret == -1) {
    if (errno == EMFILE || errno == ENFILE) {
      mrb_garbage_collect(mrb);
      ret = mrb_cloexec_pipe(mrb, pipes);
    }
  }
  return ret;
}
