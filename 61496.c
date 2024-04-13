MagickExport XMLTreeInfo *GetXMLTreeChild(XMLTreeInfo *xml_info,const char *tag)
{
  XMLTreeInfo
    *child;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  child=xml_info->child;
  if (tag != (const char *) NULL)
    while ((child != (XMLTreeInfo *) NULL) && (strcmp(child->tag,tag) != 0))
      child=child->sibling;
  return(child);
}
