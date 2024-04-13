  ExceptionInfo *magick_unused(exception))
{
  magick_unreferenced(exception);

  if (module_list == (SplayTreeInfo *) NULL)
    {
      if (module_semaphore == (SemaphoreInfo *) NULL)
        ActivateSemaphoreInfo(&module_semaphore);
      LockSemaphoreInfo(module_semaphore);
      if (module_list == (SplayTreeInfo *) NULL)
        {
          MagickBooleanType
            status;

          ModuleInfo
            *module_info;

          module_list=NewSplayTree(CompareSplayTreeString,
            (void *(*)(void *)) NULL,DestroyModuleNode);
          if (module_list == (SplayTreeInfo *) NULL)
            ThrowFatalException(ResourceLimitFatalError,
              "MemoryAllocationFailed");
          module_info=AcquireModuleInfo((const char *) NULL,"[boot-strap]");
          module_info->stealth=MagickTrue;
          status=AddValueToSplayTree(module_list,module_info->tag,module_info);
          if (status == MagickFalse)
            ThrowFatalException(ResourceLimitFatalError,
              "MemoryAllocationFailed");
          if (lt_dlinit() != 0)
            ThrowFatalException(ModuleFatalError,
              "UnableToInitializeModuleLoader");
        }
      UnlockSemaphoreInfo(module_semaphore);
    }
  return(module_list != (SplayTreeInfo *) NULL ? MagickTrue : MagickFalse);
}
