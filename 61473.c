MagickExport void ModuleComponentTerminus(void)
{
  if (module_semaphore == (SemaphoreInfo *) NULL)
    ActivateSemaphoreInfo(&module_semaphore);
  DestroyModuleList();
  DestroySemaphoreInfo(&module_semaphore);
}
