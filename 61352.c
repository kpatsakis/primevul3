static void WBMPWriteInteger(Image *image,const size_t value)
{
  int
    bits,
    flag,
    n;

  register ssize_t
    i;

  unsigned char
    buffer[5],
    octet;

  n=1;
  bits=28;
  flag=MagickFalse;
  for (i=4; i >= 0; i--)
  {
    octet=(unsigned char) ((value >> bits) & 0x7f);
    if ((flag == 0) && (octet != 0))
      {
        flag=MagickTrue;
        n=i+1;
      }
    buffer[4-i]=octet | (i && (flag || octet))*(0x01 << 7);
    bits-=7;
  }
  (void) WriteBlob(image,(size_t) n,buffer+5-n);
}
