static const char *get_top_domain(const char * const domain, size_t *outlen)
{
  size_t len = 0;
  const char *first = NULL, *last;

  if(domain) {
    len = strlen(domain);
    last = memrchr(domain, '.', len);
    if(last) {
      first = memrchr(domain, '.', (last - domain));
      if(first)
        len -= (++first - domain);
    }
  }

  if(outlen)
    *outlen = len;

  return first? first: domain;
}