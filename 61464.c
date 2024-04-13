MagickExport ModuleInfo *GetModuleInfo(const char *tag,ExceptionInfo *exception)
{
  ModuleInfo
    *module_info;

  if (IsModuleTreeInstantiated(exception) == MagickFalse)
    return((ModuleInfo *) NULL);
  LockSemaphoreInfo(module_semaphore);
  ResetSplayTreeIterator(module_list);
  if ((tag == (const char *) NULL) || (LocaleCompare(tag,"*") == 0))
    {
#if defined(MAGICKCORE_MODULES_SUPPORT)
      if (LocaleCompare(tag,"*") == 0)
        (void) OpenModules(exception);
#endif
      module_info=(ModuleInfo *) GetNextValueInSplayTree(module_list);
      UnlockSemaphoreInfo(module_semaphore);
      return(module_info);
    }
  module_info=(ModuleInfo *) GetValueFromSplayTree(module_list,tag);
  UnlockSemaphoreInfo(module_semaphore);
  return(module_info);
}
