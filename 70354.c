ModuleExport void UnregisterTIFFImage(void)
{
  (void) UnregisterMagickInfo("TIFF64");
  (void) UnregisterMagickInfo("TIFF");
  (void) UnregisterMagickInfo("TIF");
  (void) UnregisterMagickInfo("PTIF");
#if defined(MAGICKCORE_TIFF_DELEGATE)
  if (tiff_semaphore == (SemaphoreInfo *) NULL)
    ActivateSemaphoreInfo(&tiff_semaphore);
  LockSemaphoreInfo(tiff_semaphore);
  if (instantiate_key != MagickFalse)
    {
      if (DeleteMagickThreadKey(tiff_exception) == MagickFalse)
        ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
#if defined(MAGICKCORE_HAVE_TIFFMERGEFIELDINFO) && defined(MAGICKCORE_HAVE_TIFFSETTAGEXTENDER)
      if (tag_extender == (TIFFExtendProc) NULL)
        (void) TIFFSetTagExtender(tag_extender);
#endif
      (void) TIFFSetWarningHandler(warning_handler);
      (void) TIFFSetErrorHandler(error_handler);
      instantiate_key=MagickFalse;
    }
  UnlockSemaphoreInfo(tiff_semaphore);
  DestroySemaphoreInfo(&tiff_semaphore);
#endif
}
