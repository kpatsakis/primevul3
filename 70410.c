MagickExport signed int ReadBlobSignedLong(Image *image)
{
  union
  {
    unsigned int
      unsigned_value;

    signed int
      signed_value;
  } quantum;

  quantum.unsigned_value=ReadBlobLong(image);
  return(quantum.signed_value);
}
