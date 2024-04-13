MagickExport signed int ReadBlobLSBSignedLong(Image *image)
{
  union
  {
    unsigned int
      unsigned_value;

    signed int
      signed_value;
  } quantum;

  quantum.unsigned_value=ReadBlobLSBLong(image);
  return(quantum.signed_value);
}
