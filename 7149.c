__canonicalize_file_name (const char *name)
{
  return __realpath (name, NULL);
}