obtain_nonce(unsigned char* nonce)
{
  int     fd = -1;
  int     ok = -1;
  ssize_t r;

  if ((fd = open("/dev/urandom", O_RDONLY)) < 0)
    goto fail;
  if ((r = read(fd, nonce, 8)) < 0 || r != 8)
    goto fail;

  ok = 0;
 fail:
  if (fd != -1)
    close(fd);

  return (ok);
}
