MagickExport ssize_t WriteBlobFloat(Image *image,const float value)
{
  union
  {
    unsigned int
      unsigned_value;

    float
      float_value;
  } quantum;

  quantum.unsigned_value=0U;
  quantum.float_value=value;
  return(WriteBlobLong(image,quantum.unsigned_value));
}
