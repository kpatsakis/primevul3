MagickExport void MSBOrderShort(unsigned char *p,const size_t length)
{
  int
    c;

  register unsigned char
    *q;

  assert(p != (unsigned char *) NULL);
  q=p+length;
  while (p < q)
  {
    c=(int) (*p);
    *p=(*(p+1));
    p++;
    *p++=(unsigned char) c;
  }
}
