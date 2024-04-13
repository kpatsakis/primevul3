static MagickBooleanType UnregisterModule(const ModuleInfo *module_info,
  ExceptionInfo *exception)
{
  /*
    Locate and execute UnregisterFORMATImage module.
  */
  assert(module_info != (const ModuleInfo *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",module_info->tag);
  assert(exception != (ExceptionInfo *) NULL);
  if (module_info->unregister_module == NULL)
    return(MagickTrue);
  module_info->unregister_module();
  if (lt_dlclose((ModuleHandle) module_info->handle) != 0)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ModuleWarning,
        "UnableToCloseModule","`%s': %s",module_info->tag,lt_dlerror());
      return(MagickFalse);
    }
  return(MagickTrue);
}
