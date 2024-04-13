MagickExport unsigned short ReadBlobShort(Image *image)
{
  register const unsigned char
    *p;

  register unsigned short
    value;

  ssize_t
    count;

  unsigned char
    buffer[2];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  *buffer='\0';
  p=(const unsigned char *) ReadBlobStream(image,2,buffer,&count);
  if (count != 2)
    return((unsigned short) 0U);
  if (image->endian == LSBEndian)
    {
      value=(unsigned short) (*p++);
      value|=(unsigned short) (*p++) << 8;
      return(value);
    }
  value=(unsigned short) ((unsigned short) (*p++) << 8);
  value|=(unsigned short) (*p++);
  return(value);
}
