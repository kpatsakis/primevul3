suffix_requires_dir_check (char const *end)
{
  /* If END does not start with a slash, the suffix is OK.  */
  while (ISSLASH (*end))
    {
      /* Two or more slashes act like a single slash.  */
      do
        end++;
      while (ISSLASH (*end));

      switch (*end++)
        {
        default: return false;  /* An ordinary file name component is OK.  */
        case '\0': return true; /* Trailing "/" is trouble.  */
        case '.': break;        /* Possibly "." or "..".  */
        }
      /* Trailing "/.", or "/.." even if not trailing, is trouble.  */
      if (!*end || (*end == '.' && (!end[1] || ISSLASH (end[1]))))
        return true;
    }

  return false;
}