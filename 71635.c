static void *DestroyOptions(void *message)
{
  return(DestroyStringInfo((StringInfo *) message));
}
