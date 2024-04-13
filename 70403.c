MagickExport MagickSizeType ReadBlobLongLong(Image *image)
{
  MagickSizeType
    value;

  register const unsigned char
    *p;

  ssize_t
    count;

  unsigned char
    buffer[8];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  *buffer='\0';
  p=(const unsigned char *) ReadBlobStream(image,8,buffer,&count);
  if (count != 8)
    return(MagickULLConstant(0));
  if (image->endian == LSBEndian)
    {
      value=(MagickSizeType) (*p++);
      value|=(MagickSizeType) (*p++) << 8;
      value|=(MagickSizeType) (*p++) << 16;
      value|=(MagickSizeType) (*p++) << 24;
      value|=(MagickSizeType) (*p++) << 32;
      value|=(MagickSizeType) (*p++) << 40;
      value|=(MagickSizeType) (*p++) << 48;
      value|=(MagickSizeType) (*p++) << 56;
      return(value);
    }
  value=(MagickSizeType) (*p++) << 56;
  value|=(MagickSizeType) (*p++) << 48;
  value|=(MagickSizeType) (*p++) << 40;
  value|=(MagickSizeType) (*p++) << 32;
  value|=(MagickSizeType) (*p++) << 24;
  value|=(MagickSizeType) (*p++) << 16;
  value|=(MagickSizeType) (*p++) << 8;
  value|=(MagickSizeType) (*p++);
  return(value);
}
