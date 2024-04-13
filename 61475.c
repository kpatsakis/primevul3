MagickExport MagickBooleanType OpenModule(const char *module,
  ExceptionInfo *exception)
{
  char
    filename[MaxTextExtent],
    module_name[MaxTextExtent],
    name[MaxTextExtent],
    path[MaxTextExtent];

  MagickBooleanType
    status;

  ModuleHandle
    handle;

  ModuleInfo
    *module_info;

  register const CoderInfo
    *p;

  size_t
    signature;

  /*
    Assign module name from alias.
  */
  assert(module != (const char *) NULL);
  module_info=(ModuleInfo *) GetModuleInfo(module,exception);
  if (module_info != (ModuleInfo *) NULL)
    return(MagickTrue);
  (void) CopyMagickString(module_name,module,MaxTextExtent);
  p=GetCoderInfo(module,exception);
  if (p != (CoderInfo *) NULL)
    (void) CopyMagickString(module_name,p->name,MaxTextExtent);
  if (GetValueFromSplayTree(module_list,module_name) != (void *) NULL)
    return(MagickTrue);  /* module already opened, return */
  /*
    Locate module.
  */
  handle=(ModuleHandle) NULL;
  TagToCoderModuleName(module_name,filename);
  (void) LogMagickEvent(ModuleEvent,GetMagickModule(),
    "Searching for module \"%s\" using filename \"%s\"",module_name,filename);
  *path='\0';
  status=GetMagickModulePath(filename,MagickImageCoderModule,path,exception);
  if (status == MagickFalse)
    return(MagickFalse);
  /*
    Load module
  */
  (void) LogMagickEvent(ModuleEvent,GetMagickModule(),
    "Opening module at path \"%s\"",path);
  handle=(ModuleHandle) lt_dlopen(path);
  if (handle == (ModuleHandle) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
        "UnableToLoadModule","`%s': %s",path,lt_dlerror());
      return(MagickFalse);
    }
  /*
    Register module.
  */
  module_info=AcquireModuleInfo(path,module_name);
  module_info->handle=handle;
  if (RegisterModule(module_info,exception) == (ModuleInfo *) NULL)
    return(MagickFalse);
  /*
    Define RegisterFORMATImage method.
  */
  TagToModuleName(module_name,"Register%sImage",name);
  module_info->register_module=(size_t (*)(void)) lt_dlsym(handle,name);
  if (module_info->register_module == (size_t (*)(void)) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
        "UnableToRegisterImageFormat","`%s': %s",module_name,lt_dlerror());
      return(MagickFalse);
    }
  (void) LogMagickEvent(ModuleEvent,GetMagickModule(),
    "Method \"%s\" in module \"%s\" at address %p",name,module_name,
    (void *) module_info->register_module);
  /*
    Define UnregisterFORMATImage method.
  */
  TagToModuleName(module_name,"Unregister%sImage",name);
  module_info->unregister_module=(void (*)(void)) lt_dlsym(handle,name);
  if (module_info->unregister_module == (void (*)(void)) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
        "UnableToRegisterImageFormat","`%s': %s",module_name,lt_dlerror());
      return(MagickFalse);
    }
  (void) LogMagickEvent(ModuleEvent,GetMagickModule(),
    "Method \"%s\" in module \"%s\" at address %p",name,module_name,
    (void *) module_info->unregister_module);
  signature=module_info->register_module();
  if (signature != MagickImageCoderSignature)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleError,
        "ImageCoderSignatureMismatch","`%s': %8lx != %8lx",module_name,
        (unsigned long) signature,(unsigned long) MagickImageCoderSignature);
      return(MagickFalse);
    }
  return(MagickTrue);
}
