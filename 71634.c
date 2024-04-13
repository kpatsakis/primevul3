MagickExport LinkedListInfo *DestroyLocaleOptions(LinkedListInfo *messages)
{
  assert(messages != (LinkedListInfo *) NULL);
  (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  return(DestroyLinkedList(messages,DestroyOptions));
}
