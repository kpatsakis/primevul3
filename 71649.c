static MagickBooleanType LoadLocaleCache(SplayTreeInfo *cache,const char *xml,
  const char *filename,const char *locale,const size_t depth,ExceptionInfo *exception)
{
  char
    keyword[MagickLocaleExtent],
    message[MagickLocaleExtent],
    tag[MagickLocaleExtent],
    *token;

  const char
    *q;

  FatalErrorHandler
    fatal_handler;

  LocaleInfo
    *locale_info;

  MagickStatusType
    status;

  register char
    *p;

  size_t
    extent;

  /*
    Read the locale configure file.
  */
  (void) LogMagickEvent(ConfigureEvent,GetMagickModule(),
    "Loading locale configure file \"%s\" ...",filename);
  if (xml == (const char *) NULL)
    return(MagickFalse);
  status=MagickTrue;
  locale_info=(LocaleInfo *) NULL;
  *tag='\0';
  *message='\0';
  *keyword='\0';
  fatal_handler=SetFatalErrorHandler(LocaleFatalErrorHandler);
  token=AcquireString(xml);
  extent=strlen(token)+MagickPathExtent;
  for (q=(char *) xml; *q != '\0'; )
  {
    /*
      Interpret XML.
    */
    GetNextToken(q,&q,extent,token);
    if (*token == '\0')
      break;
    (void) CopyMagickString(keyword,token,MagickLocaleExtent);
    if (LocaleNCompare(keyword,"<!DOCTYPE",9) == 0)
      {
        /*
          Doctype element.
        */
        while ((LocaleNCompare(q,"]>",2) != 0) && (*q != '\0'))
        {
          GetNextToken(q,&q,extent,token);
          while (isspace((int) ((unsigned char) *q)) != 0)
            q++;
        }
        continue;
      }
    if (LocaleNCompare(keyword,"<!--",4) == 0)
      {
        /*
          Comment element.
        */
        while ((LocaleNCompare(q,"->",2) != 0) && (*q != '\0'))
        {
          GetNextToken(q,&q,extent,token);
          while (isspace((int) ((unsigned char) *q)) != 0)
            q++;
        }
        continue;
      }
    if (LocaleCompare(keyword,"<include") == 0)
      {
        /*
          Include element.
        */
        while (((*token != '/') && (*(token+1) != '>')) && (*q != '\0'))
        {
          (void) CopyMagickString(keyword,token,MagickLocaleExtent);
          GetNextToken(q,&q,extent,token);
          if (*token != '=')
            continue;
          GetNextToken(q,&q,extent,token);
          if (LocaleCompare(keyword,"locale") == 0)
            {
              if (LocaleCompare(locale,token) != 0)
                break;
              continue;
            }
          if (LocaleCompare(keyword,"file") == 0)
            {
              if (depth > MagickMaxRecursionDepth)
                (void) ThrowMagickException(exception,GetMagickModule(),
                  ConfigureError,"IncludeElementNestedTooDeeply","`%s'",token);
              else
                {
                  char
                    path[MagickPathExtent],
                    *file_xml;

                  *path='\0';
                  GetPathComponent(filename,HeadPath,path);
                  if (*path != '\0')
                    (void) ConcatenateMagickString(path,DirectorySeparator,
                      MagickPathExtent);
                  if (*token == *DirectorySeparator)
                    (void) CopyMagickString(path,token,MagickPathExtent);
                  else
                    (void) ConcatenateMagickString(path,token,MagickPathExtent);
                  file_xml=FileToXML(path,~0UL);
                  if (file_xml != (char *) NULL)
                    {
                      status&=LoadLocaleCache(cache,file_xml,path,locale,
                        depth+1,exception);
                      file_xml=DestroyString(file_xml);
                    }
                }
            }
        }
        continue;
      }
    if (LocaleCompare(keyword,"<locale") == 0)
      {
        /*
          Locale element.
        */
        while ((*token != '>') && (*q != '\0'))
        {
          (void) CopyMagickString(keyword,token,MagickLocaleExtent);
          GetNextToken(q,&q,extent,token);
          if (*token != '=')
            continue;
          GetNextToken(q,&q,extent,token);
        }
        continue;
      }
    if (LocaleCompare(keyword,"</locale>") == 0)
      {
        ChopLocaleComponents(tag,1);
        (void) ConcatenateMagickString(tag,"/",MagickLocaleExtent);
        continue;
      }
    if (LocaleCompare(keyword,"<localemap>") == 0)
      continue;
    if (LocaleCompare(keyword,"</localemap>") == 0)
      continue;
    if (LocaleCompare(keyword,"<message") == 0)
      {
        /*
          Message element.
        */
        while ((*token != '>') && (*q != '\0'))
        {
          (void) CopyMagickString(keyword,token,MagickLocaleExtent);
          GetNextToken(q,&q,extent,token);
          if (*token != '=')
            continue;
          GetNextToken(q,&q,extent,token);
          if (LocaleCompare(keyword,"name") == 0)
            {
              (void) ConcatenateMagickString(tag,token,MagickLocaleExtent);
              (void) ConcatenateMagickString(tag,"/",MagickLocaleExtent);
            }
        }
        for (p=(char *) q; (*q != '<') && (*q != '\0'); q++) ;
        while (isspace((int) ((unsigned char) *p)) != 0)
          p++;
        q--;
        while ((isspace((int) ((unsigned char) *q)) != 0) && (q > p))
          q--;
        (void) CopyMagickString(message,p,MagickMin((size_t) (q-p+2),
          MagickLocaleExtent));
        locale_info=(LocaleInfo *) AcquireCriticalMemory(sizeof(*locale_info));
        (void) memset(locale_info,0,sizeof(*locale_info));
        locale_info->path=ConstantString(filename);
        locale_info->tag=ConstantString(tag);
        locale_info->message=ConstantString(message);
        locale_info->signature=MagickCoreSignature;
        status=AddValueToSplayTree(cache,locale_info->tag,locale_info);
        if (status == MagickFalse)
          (void) ThrowMagickException(exception,GetMagickModule(),
            ResourceLimitError,"MemoryAllocationFailed","`%s'",
            locale_info->tag);
        (void) ConcatenateMagickString(tag,message,MagickLocaleExtent);
        (void) ConcatenateMagickString(tag,"\n",MagickLocaleExtent);
        q++;
        continue;
      }
    if (LocaleCompare(keyword,"</message>") == 0)
      {
        ChopLocaleComponents(tag,2);
        (void) ConcatenateMagickString(tag,"/",MagickLocaleExtent);
        continue;
      }
    if (*keyword == '<')
      {
        /*
          Subpath element.
        */
        if (*(keyword+1) == '?')
          continue;
        if (*(keyword+1) == '/')
          {
            ChopLocaleComponents(tag,1);
            if (*tag != '\0')
              (void) ConcatenateMagickString(tag,"/",MagickLocaleExtent);
            continue;
          }
        token[strlen(token)-1]='\0';
        (void) CopyMagickString(token,token+1,MagickLocaleExtent);
        (void) ConcatenateMagickString(tag,token,MagickLocaleExtent);
        (void) ConcatenateMagickString(tag,"/",MagickLocaleExtent);
        continue;
      }
    GetNextToken(q,(const char **) NULL,extent,token);
    if (*token != '=')
      continue;
  }
  token=(char *) RelinquishMagickMemory(token);
  (void) SetFatalErrorHandler(fatal_handler);
  return(status != 0 ? MagickTrue : MagickFalse);
}
