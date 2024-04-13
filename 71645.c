MagickExport const char *GetLocaleValue(const LocaleInfo *locale_info)
{
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  assert(locale_info != (LocaleInfo *) NULL);
  assert(locale_info->signature == MagickCoreSignature);
  return(locale_info->message);
}
