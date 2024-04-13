ModuleExport size_t RegisterMVGImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("MVG");
  entry->decoder=(DecodeImageHandler *) ReadMVGImage;
  entry->encoder=(EncodeImageHandler *) WriteMVGImage;
  entry->magick=(IsImageFormatHandler *) IsMVG;
  entry->adjoin=MagickFalse;
  entry->seekable_stream=MagickTrue;
  entry->description=ConstantString("Magick Vector Graphics");
  entry->module=ConstantString("MVG");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
