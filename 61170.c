ModuleExport size_t RegisterJBIGImage(void)
{
#define JBIGDescription  "Joint Bi-level Image experts Group interchange format"

  char
    version[MaxTextExtent];

  MagickInfo
    *entry;

  *version='\0';
#if defined(JBG_VERSION)
  (void) CopyMagickString(version,JBG_VERSION,MaxTextExtent);
#endif
  entry=SetMagickInfo("BIE");
#if defined(MAGICKCORE_JBIG_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJBIGImage;
  entry->encoder=(EncodeImageHandler *) WriteJBIGImage;
#endif
  entry->adjoin=MagickFalse;
  entry->description=ConstantString(JBIGDescription);
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("JBIG");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("JBG");
#if defined(MAGICKCORE_JBIG_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJBIGImage;
  entry->encoder=(EncodeImageHandler *) WriteJBIGImage;
#endif
  entry->description=ConstantString(JBIGDescription);
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("JBIG");
  (void) RegisterMagickInfo(entry);
  entry=SetMagickInfo("JBIG");
#if defined(MAGICKCORE_JBIG_DELEGATE)
  entry->decoder=(DecodeImageHandler *) ReadJBIGImage;
  entry->encoder=(EncodeImageHandler *) WriteJBIGImage;
#endif
  entry->description=ConstantString(JBIGDescription);
  if (*version != '\0')
    entry->version=ConstantString(version);
  entry->module=ConstantString("JBIG");
  (void) RegisterMagickInfo(entry);
  return(MagickImageCoderSignature);
}
