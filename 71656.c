static int LocaleTagCompare(const void *x,const void *y)
{
  register char
    **p,
    **q;

  p=(char **) x;
  q=(char **) y;
  return(LocaleCompare(*p,*q));
}
