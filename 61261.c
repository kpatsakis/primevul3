ModuleExport size_t RegisterSIXELImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("SIXEL");
  entry->decoder=(DecodeImageHandler *) ReadSIXELImage;
  entry->encoder=(EncodeImageHandler *) WriteSIXELImage;
  entry->magick=(IsImageFormatHandler *) IsSIXEL;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("DEC SIXEL Graphics Format");
  entry->module=ConstantString("SIXEL");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("SIX");
  entry->decoder=(DecodeImageHandler *) ReadSIXELImage;
  entry->encoder=(EncodeImageHandler *) WriteSIXELImage;
  entry->magick=(IsImageFormatHandler *) IsSIXEL;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("DEC SIXEL Graphics Format");
  entry->module=ConstantString("SIX");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
