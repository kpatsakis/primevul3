MagickExport ssize_t WriteBlobLSBShort(Image *image,const unsigned short value)
{
  unsigned char
    buffer[2];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  buffer[0]=(unsigned char) value;
  buffer[1]=(unsigned char) (value >> 8);
  return(WriteBlobStream(image,2,buffer));
}
