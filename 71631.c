static void ChopLocaleComponents(char *path,const size_t components)
{
  register char
    *p;

  ssize_t
    count;

  if (*path == '\0')
    return;
  p=path+strlen(path)-1;
  if (*p == '/')
    *p='\0';
  for (count=0; (count < (ssize_t) components) && (p > path); p--)
    if (*p == '/')
      {
        *p='\0';
        count++;
      }
  if (count < (ssize_t) components)
    *path='\0';
}
