is_path_absolute(const char *path)	/* I - Input path */
{
 /*
  * Check for a leading slash...
  */

  if (path[0] != '/')
    return (0);

 /*
  * Check for "<" or quotes in the path and reject since this is probably
  * someone trying to inject HTML...
  */

  if (strchr(path, '<') != NULL || strchr(path, '\"') != NULL || strchr(path, '\'') != NULL)
    return (0);

 /*
  * Check for "/.." in the path...
  */

  while ((path = strstr(path, "/..")) != NULL)
  {
    if (!path[3] || path[3] == '/')
      return (0);

    path ++;
  }

 /*
  * If we haven't found any relative paths, return 1 indicating an
  * absolute path...
  */

  return (1);
}
