dir_check (char *dir, char *dirend)
{
  strcpy (dirend, dir_suffix);
  return file_accessible (dir);
}