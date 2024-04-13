MagickExport ImageType GetImageType(const Image *image,ExceptionInfo *exception)
{
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  if (image->colorspace == CMYKColorspace)
    {
      if (image->matte == MagickFalse)
        return(ColorSeparationType);
      return(ColorSeparationMatteType);
    }
  if (IsMonochromeImage(image,exception) != MagickFalse)
    return(BilevelType);
  if (IsGrayImage(image,exception) != MagickFalse)
    {
      if (image->matte != MagickFalse)
        return(GrayscaleMatteType);
      return(GrayscaleType);
    }
  if (IsPaletteImage(image,exception) != MagickFalse)
    {
      if (image->matte != MagickFalse)
        return(PaletteMatteType);
      return(PaletteType);
    }
  if (image->matte != MagickFalse)
    return(TrueColorMatteType);
  return(TrueColorType);
}
