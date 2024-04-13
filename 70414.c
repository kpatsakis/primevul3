MagickExport BlobInfo *ReferenceBlob(BlobInfo *blob)
{
  assert(blob != (BlobInfo *) NULL);
  assert(blob->signature == MagickCoreSignature);
  if (blob->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  LockSemaphoreInfo(blob->semaphore);
  blob->reference_count++;
  UnlockSemaphoreInfo(blob->semaphore);
  return(blob);
}
