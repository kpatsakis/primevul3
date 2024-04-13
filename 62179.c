MagickExport MagickBooleanType IsGrayImage(const Image *image,
  ExceptionInfo *exception)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if ((image->type == BilevelType) || (image->type == GrayscaleType) ||
      (image->type == GrayscaleMatteType))
    return(MagickTrue);
  return(MagickFalse);
}
