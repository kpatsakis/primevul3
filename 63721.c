local_wcsnlen (const wchar_t *s, size_t maxlen)
{
  const wchar_t *ptr;

  for (ptr = s; maxlen > 0 && *ptr != (wchar_t) 0; ptr++, maxlen--)
    ;
  return ptr - s;
}
