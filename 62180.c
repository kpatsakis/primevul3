MagickExport MagickBooleanType IsMonochromeImage(const Image *image,
  ExceptionInfo *exception)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->type == BilevelType)
    return(MagickTrue);
  return(MagickFalse);
}
