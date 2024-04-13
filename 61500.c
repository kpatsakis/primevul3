MagickExport const char **GetXMLTreeProcessingInstructions(
  XMLTreeInfo *xml_info,const char *target)
{
  register ssize_t
    i;

  XMLTreeRoot
    *root;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  root=(XMLTreeRoot *) xml_info;
  while (root->root.parent != (XMLTreeInfo *) NULL)
    root=(XMLTreeRoot *) root->root.parent;
  i=0;
  while ((root->processing_instructions[i] != (char **) NULL) &&
         (strcmp(root->processing_instructions[i][0],target) != 0))
    i++;
  if (root->processing_instructions[i] == (char **) NULL)
    return((const char **) sentinel);
  return((const char **) (root->processing_instructions[i]+1));
}
