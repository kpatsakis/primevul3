mrb_dup(mrb_state *mrb, int fd, mrb_bool *failed)
{
  int new_fd;

  *failed = FALSE;
  if (fd < 0)
    return fd;

  new_fd = dup(fd);
  if (new_fd == -1) *failed = TRUE;
  return new_fd;
}
