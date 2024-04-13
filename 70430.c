MagickExport ssize_t WriteBlobMSBLong(Image *image,const unsigned int value)
{
  unsigned char
    buffer[4];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  buffer[0]=(unsigned char) (value >> 24);
  buffer[1]=(unsigned char) (value >> 16);
  buffer[2]=(unsigned char) (value >> 8);
  buffer[3]=(unsigned char) value;
  return(WriteBlobStream(image,4,buffer));
}
