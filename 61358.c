ModuleExport size_t RegisterWMFImage(void)
{
  MagickInfo
    *entry;

  entry = SetMagickInfo("WMZ");
#if defined(MAGICKCORE_WMF_DELEGATE) || defined(MAGICKCORE_WMFLITE_DELEGATE)
  entry->decoder=ReadWMFImage;
#endif
  entry->description=ConstantString("Compressed Windows Meta File");
  entry->module=ConstantString("WMZ");
  entry->seekable_stream=MagickTrue;
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("WMF");
#if defined(MAGICKCORE_WMF_DELEGATE) || defined(MAGICKCORE_WMFLITE_DELEGATE)
  entry->decoder=ReadWMFImage;
#endif
  entry->description=ConstantString("Windows Meta File");
  entry->module=ConstantString("WMF");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
