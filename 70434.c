MagickExport ssize_t WriteBlobShort(Image *image,const unsigned short value)
{
  unsigned char
    buffer[2];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  if (image->endian == LSBEndian)
    {
      buffer[0]=(unsigned char) value;
      buffer[1]=(unsigned char) (value >> 8);
      return(WriteBlobStream(image,2,buffer));
    }
  buffer[0]=(unsigned char) (value >> 8);
  buffer[1]=(unsigned char) value;
  return(WriteBlobStream(image,2,buffer));
}
