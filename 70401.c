MagickExport signed short ReadBlobLSBSignedShort(Image *image)
{
  union
  {
    unsigned short
      unsigned_value;

    signed short
      signed_value;
  } quantum;

  quantum.unsigned_value=ReadBlobLSBShort(image);
  return(quantum.signed_value);
}
