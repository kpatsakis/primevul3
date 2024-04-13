static int LocaleInfoCompare(const void *x,const void *y)
{
  const LocaleInfo
    **p,
    **q;

  p=(const LocaleInfo **) x,
  q=(const LocaleInfo **) y;
  if (LocaleCompare((*p)->path,(*q)->path) == 0)
    return(LocaleCompare((*p)->tag,(*q)->tag));
  return(LocaleCompare((*p)->path,(*q)->path));
}
