static SplayTreeInfo *AcquireLocaleSplayTree(const char *filename,
  const char *locale,ExceptionInfo *exception)
{
  MagickStatusType
    status;

  SplayTreeInfo
    *cache;

  cache=NewSplayTree(CompareSplayTreeString,(void *(*)(void *)) NULL,
    DestroyLocaleNode);
  status=MagickTrue;
#if !defined(MAGICKCORE_ZERO_CONFIGURATION_SUPPORT)
  {
    const StringInfo
      *option;

    LinkedListInfo
      *options;

    options=GetLocaleOptions(filename,exception);
    option=(const StringInfo *) GetNextValueInLinkedList(options);
    while (option != (const StringInfo *) NULL)
    {
      status&=LoadLocaleCache(cache,(const char *)
        GetStringInfoDatum(option),GetStringInfoPath(option),locale,0,
        exception);
      option=(const StringInfo *) GetNextValueInLinkedList(options);
    }
    options=DestroyLocaleOptions(options);
    if (GetNumberOfNodesInSplayTree(cache) == 0)
      {
        options=GetLocaleOptions("english.xml",exception);
        option=(const StringInfo *) GetNextValueInLinkedList(options);
        while (option != (const StringInfo *) NULL)
        {
          status&=LoadLocaleCache(cache,(const char *)
            GetStringInfoDatum(option),GetStringInfoPath(option),locale,0,
            exception);
          option=(const StringInfo *) GetNextValueInLinkedList(options);
        }
        options=DestroyLocaleOptions(options);
      }
  }
#endif
  if (GetNumberOfNodesInSplayTree(cache) == 0)
    status&=LoadLocaleCache(cache,LocaleMap,"built-in",locale,0,
      exception);
  return(cache);
}
