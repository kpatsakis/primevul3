MagickExport signed short ReadBlobMSBSignedShort(Image *image)
{
  union
  {
    unsigned short
      unsigned_value;

    signed short
      signed_value;
  } quantum;

  quantum.unsigned_value=ReadBlobMSBShort(image);
  return(quantum.signed_value);
}
