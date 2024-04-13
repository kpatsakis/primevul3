MagickExport XMLTreeInfo *DestroyXMLTree(XMLTreeInfo *xml_info)
{
  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  DestroyXMLTreeChild(xml_info);
  DestroyXMLTreeOrdered(xml_info);
  DestroyXMLTreeRoot(xml_info);
  xml_info->attributes=DestroyXMLTreeAttributes(xml_info->attributes);
  xml_info->content=DestroyString(xml_info->content);
  xml_info->tag=DestroyString(xml_info->tag);
  xml_info=(XMLTreeInfo *) RelinquishMagickMemory(xml_info);
  return((XMLTreeInfo *) NULL);
}
