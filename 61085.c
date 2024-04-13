static MagickBooleanType WriteCLIPBOARDImage(const ImageInfo *image_info,
  Image *image)
{
  /*
    Allocate memory for pixels.
  */
  assert(image_info != (const ImageInfo *) NULL);
  assert(image_info->signature == MagickSignature);
  assert(image != (Image *) NULL);
  assert(image->signature == MagickSignature);
  if (image->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",image->filename);
  {
    HBITMAP
      bitmapH;

    OpenClipboard(NULL);
    EmptyClipboard();
    bitmapH=(HBITMAP) ImageToHBITMAP(image,&image->exception);
    SetClipboardData(CF_BITMAP,bitmapH);
    CloseClipboard();
  }
  return(MagickTrue);
}
