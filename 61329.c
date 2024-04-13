ModuleExport size_t RegisterTTFImage(void)
{
  char
    version[MaxTextExtent];

  MagickInfo
    *entry;

  *version='\0';
#if defined(FREETYPE_MAJOR) && defined(FREETYPE_MINOR) && defined(FREETYPE_PATCH)
  (void) FormatLocaleString(version,MaxTextExtent,"Freetype %d.%d.%d",
    FREETYPE_MAJOR,FREETYPE_MINOR,FREETYPE_PATCH);
#endif
  entry=SetMagickInfo("DFONT");
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadTTFImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsTTF;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Multi-face font package");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("TTF");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("PFA");
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadTTFImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsPFA;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Postscript Type 1 font (ASCII)");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("TTF");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("PFB");
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadTTFImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsPFA;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Postscript Type 1 font (binary)");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("TTF");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("OTF");
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadTTFImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsTTF;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("Open Type font");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("TTF");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("TTC");
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadTTFImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsTTF;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("TrueType font collection");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("TTF");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("TTF");
#if defined(MAGICKCORE_FREETYPE_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadTTFImage;
#endif
  entry->magick=(IsImageFormatHandler *) IsTTF;
  entry->adjoin=MagickFalse;
  entry->description=ConstantString("TrueType font");
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("TTF");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
