MagickExport const char *GetXMLTreeContent(XMLTreeInfo *xml_info)
{
  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  return(xml_info->content);
}
