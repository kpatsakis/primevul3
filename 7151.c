__old_realpath (const char *name, char *resolved)
{
  if (resolved == NULL)
    {
      __set_errno (EINVAL);
      return NULL;
    }

  return __realpath (name, resolved);
}