MagickExport signed short ReadBlobSignedShort(Image *image)
{
  union
  {
    unsigned short
      unsigned_value;

    signed short
      signed_value;
  } quantum;

  quantum.unsigned_value=ReadBlobShort(image);
  return(quantum.signed_value);
}
