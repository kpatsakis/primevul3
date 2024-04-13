static void TagToCoderModuleName(const char *tag,char *name)
{
  assert(tag != (char *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",tag);
  assert(name != (char *) NULL);
#if defined(MAGICKCORE_LTDL_DELEGATE)
  (void) FormatLocaleString(name,MaxTextExtent,"%s.la",tag);
  (void) LocaleLower(name);
#else
#if defined(MAGICKCORE_WINDOWS_SUPPORT)
  if (LocaleNCompare("IM_MOD_",tag,7) == 0)
    (void) CopyMagickString(name,tag,MaxTextExtent);
  else
    {
#if defined(_DEBUG)
      (void) FormatLocaleString(name,MaxTextExtent,"IM_MOD_DB_%s_.dll",tag);
#else
      (void) FormatLocaleString(name,MaxTextExtent,"IM_MOD_RL_%s_.dll",tag);
#endif
    }
#endif
#endif
}
