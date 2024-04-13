MagickExport XMLTreeInfo *AddChildToXMLTree(XMLTreeInfo *xml_info,
  const char *tag,const size_t offset)
{
  XMLTreeInfo
    *child;

  if (xml_info == (XMLTreeInfo *) NULL)
    return((XMLTreeInfo *) NULL);
  child=(XMLTreeInfo *) AcquireMagickMemory(sizeof(*child));
  if (child == (XMLTreeInfo *) NULL)
    return((XMLTreeInfo *) NULL);
  (void) ResetMagickMemory(child,0,sizeof(*child));
  child->tag=ConstantString(tag);
  child->attributes=sentinel;
  child->content=ConstantString("");
  child->debug=IsEventLogging();
  child->signature=MagickSignature;
  return(InsertTagIntoXMLTree(xml_info,child,offset));
}
