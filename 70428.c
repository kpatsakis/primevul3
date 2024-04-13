MagickExport ssize_t WriteBlobLSBSignedShort(Image *image,
  const signed short value)
{
  union
  {
    unsigned short
      unsigned_value;

    signed short
      signed_value;
  } quantum;

  unsigned char
    buffer[2];

  assert(image != (Image *) NULL);
  assert(image->signature == MagickCoreSignature);
  quantum.signed_value=value;
  buffer[0]=(unsigned char) quantum.unsigned_value;
  buffer[1]=(unsigned char) (quantum.unsigned_value >> 8);
  return(WriteBlobStream(image,2,buffer));
}
