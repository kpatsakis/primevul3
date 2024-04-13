static const ModuleInfo *RegisterModule(const ModuleInfo *module_info,
  ExceptionInfo *exception)
{
  MagickBooleanType
    status;

  assert(module_info != (ModuleInfo *) NULL);
  assert(module_info->signature == MagickSignature);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"%s",module_info->tag);
  if (module_list == (SplayTreeInfo *) NULL)
    return((const ModuleInfo *) NULL);
  status=AddValueToSplayTree(module_list,module_info->tag,module_info);
  if (status == MagickFalse)
    (void) ThrowMagickException(exception,GetMagickModule(),ResourceLimitError,
      "MemoryAllocationFailed","`%s'",module_info->tag);
  return(module_info);
}
