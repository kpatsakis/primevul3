MagickPrivate MagickBooleanType ClearExceptionInfo(ExceptionInfo *exception,
  MagickBooleanType relinquish)
{
  assert(exception != (ExceptionInfo *) NULL);
  assert(exception->signature == MagickSignature);
  if (exception->semaphore == (SemaphoreInfo *) NULL)
    ActivateSemaphoreInfo(&exception->semaphore);
  LockSemaphoreInfo(exception->semaphore);
  if (relinquish == MagickFalse)
    relinquish=exception->relinquish;
  exception->severity=UndefinedException;
  if (relinquish != MagickFalse)
    {
      exception->signature=(~MagickSignature);
      if (exception->exceptions != (void *) NULL)
        exception->exceptions=(void *) DestroyLinkedList((LinkedListInfo *)
          exception->exceptions,DestroyExceptionElement);
    }
  else if (exception->exceptions != (void *) NULL)
    ClearLinkedList((LinkedListInfo *) exception->exceptions,
      DestroyExceptionElement);
  UnlockSemaphoreInfo(exception->semaphore);
  if (relinquish != MagickFalse)
    DestroySemaphoreInfo(&exception->semaphore);
  return(relinquish);
}
