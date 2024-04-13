ModuleExport size_t RegisterFPXImage(void)
{
  MagickInfo
    *entry;

  entry=SetMagickInfo("FPX");
#if defined(MAGICKCORE_FPX_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadFPXImage;
  entry->encoder=(EncodeImageHandler *) WriteFPXImage;
#endif
  entry->adjoin=MagickFalse;
  entry->seekable_stream=MagickTrue;
  entry->blob_support=MagickFalse;
  entry->description=ConstantString("FlashPix Format");
  entry->module=ConstantString("FPX");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
