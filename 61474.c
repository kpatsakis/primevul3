static int ModuleInfoCompare(const void *x,const void *y)
{
  const ModuleInfo
    **p,
    **q;

  p=(const ModuleInfo **) x,
  q=(const ModuleInfo **) y;
  if (LocaleCompare((*p)->path,(*q)->path) == 0)
    return(LocaleCompare((*p)->tag,(*q)->tag));
  return(LocaleCompare((*p)->path,(*q)->path));
}
