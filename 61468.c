MagickExport MagickBooleanType InvokeDynamicImageFilter(const char *tag,
  Image **images,const int argc,const char **argv,ExceptionInfo *exception)
{
  char
    name[MaxTextExtent],
    path[MaxTextExtent];

  ImageFilterHandler
    *image_filter;

  MagickBooleanType
    status;

  ModuleHandle
    handle;

  PolicyRights
    rights;

  /*
    Find the module.
  */
  assert(images != (Image **) NULL);
  assert((*images)->signature == MagickSignature);
  if ((*images)->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",
      (*images)->filename);
#if !defined(MAGICKCORE_BUILD_MODULES)
  {
    MagickBooleanType
      status;

    status=InvokeStaticImageFilter(tag,images,argc,argv,exception);
    if (status != MagickFalse)
      return(status);
  }
#endif
  rights=ReadPolicyRights;
  if (IsRightsAuthorized(FilterPolicyDomain,rights,tag) == MagickFalse)
    {
      errno=EPERM;
      (void) ThrowMagickException(exception,GetMagickModule(),PolicyError,
        "NotAuthorized","`%s'",tag);
      return(MagickFalse);
    }
  TagToFilterModuleName(tag,name);
  status=GetMagickModulePath(name,MagickImageFilterModule,path,exception);
  if (status == MagickFalse)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
        "UnableToLoadModule","`%s': %s",name,path);
      return(MagickFalse);
    }
  /*
    Open the module.
  */
  handle=(ModuleHandle) lt_dlopen(path);
  if (handle == (ModuleHandle) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
        "UnableToLoadModule","`%s': %s",name,lt_dlerror());
      return(MagickFalse);
    }
  /*
    Locate the module.
  */
#if !defined(MAGICKCORE_NAMESPACE_PREFIX)
  (void) FormatLocaleString(name,MaxTextExtent,"%sImage",tag);
#else
  (void) FormatLocaleString(name,MaxTextExtent,"%s%sImage",
    MAGICKCORE_NAMESPACE_PREFIX,tag);
#endif
  /*
    Execute the module.
  */
  ClearMagickException(exception);
  image_filter=(ImageFilterHandler *) lt_dlsym(handle,name);
  if (image_filter == (ImageFilterHandler *) NULL)
    (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
      "UnableToLoadModule","`%s': %s",name,lt_dlerror());
  else
    {
      size_t
        signature;

      if ((*images)->debug != MagickFalse)
        (void) LogMagickEvent(ModuleEvent,GetMagickModule(),
          "Invoking \"%s\" dynamic image filter",tag);
      signature=image_filter(images,argc,argv,exception);
      if ((*images)->debug != MagickFalse)
        (void) LogMagickEvent(ModuleEvent,GetMagickModule(),"\"%s\" completes",
          tag);
      if (signature != MagickImageFilterSignature)
        (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
          "ImageFilterSignatureMismatch","`%s': %8lx != %8lx",tag,
          (unsigned long) signature,(unsigned long) MagickImageFilterSignature);
    }
  /*
    Close the module.
  */
  if (lt_dlclose(handle) != 0)
    (void) ThrowMagickException(exception,GetMagickModule(),ModuleWarning,
      "UnableToCloseModule","`%s': %s",name,lt_dlerror());
  return(exception->severity < ErrorException ? MagickTrue : MagickFalse);
}
