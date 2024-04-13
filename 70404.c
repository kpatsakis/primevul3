MagickExport unsigned int ReadBlobMSBLong(Image *image)
{
  register const unsigned char
    *p;

  register unsigned int
    value;

  ssize_t
    count;

  unsigned char
    buffer[4];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  *buffer='\0';
  p=(const unsigned char *) ReadBlobStream(image,4,buffer,&count);
  if (count != 4)
    return(0UL);
  value=(unsigned int) (*p++) << 24;
  value|=(unsigned int) (*p++) << 16;
  value|=(unsigned int) (*p++) << 8;
  value|=(unsigned int) (*p++);
  return(value);
}
