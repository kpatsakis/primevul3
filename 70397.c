MagickExport float ReadBlobFloat(Image *image)
{
  union
  {
    unsigned int
      unsigned_value;

    float
      float_value;
  } quantum;

  quantum.float_value=0.0;
  quantum.unsigned_value=ReadBlobLong(image);
  return(quantum.float_value);
}
