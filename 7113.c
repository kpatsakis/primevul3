static bool bad_domain(const char *domain)
{
  if(strcasecompare(domain, "localhost"))
    return FALSE;
  else {
    /* there must be a dot present, but that dot must not be a trailing dot */
    char *dot = strchr(domain, '.');
    if(dot)
      return dot[1] ? FALSE : TRUE;
  }
  return TRUE;
}