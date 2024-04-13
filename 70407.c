MagickExport signed int ReadBlobMSBSignedLong(Image *image)
{
  union
  {
    unsigned int
      unsigned_value;

    signed int
      signed_value;
  } quantum;

  quantum.unsigned_value=ReadBlobMSBLong(image);
  return(quantum.signed_value);
}
