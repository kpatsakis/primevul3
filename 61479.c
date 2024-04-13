static void TagToFilterModuleName(const char *tag,char *name)
{
  assert(tag != (char *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",tag);
  assert(name != (char *) NULL);
#if !defined(MAGICKCORE_LTDL_DELEGATE)
  (void) FormatLocaleString(name,MaxTextExtent,"%s.dll",tag);
#else
  (void) FormatLocaleString(name,MaxTextExtent,"%s.la",tag);
#endif
}
