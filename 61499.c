MagickExport XMLTreeInfo *GetXMLTreePath(XMLTreeInfo *xml_info,const char *path)
{
  char
    **components,
    subnode[MaxTextExtent],
    tag[MaxTextExtent];

  register ssize_t
    i;

  size_t
    number_components;

  ssize_t
    j;

  XMLTreeInfo
    *node;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  node=xml_info;
  components=GetPathComponents(path,&number_components);
  if (components == (char **) NULL)
    return((XMLTreeInfo *) NULL);
  for (i=0; i < (ssize_t) number_components; i++)
  {
    GetPathComponent(components[i],SubimagePath,subnode);
    GetPathComponent(components[i],CanonicalPath,tag);
    node=GetXMLTreeChild(node,tag);
    if (node == (XMLTreeInfo *) NULL)
      break;
    for (j=(ssize_t) StringToLong(subnode)-1; j > 0; j--)
    {
      node=GetXMLTreeOrdered(node);
      if (node == (XMLTreeInfo *) NULL)
        break;
    }
    if (node == (XMLTreeInfo *) NULL)
      break;
    components[i]=DestroyString(components[i]);
  }
  for ( ; i < (ssize_t) number_components; i++)
    components[i]=DestroyString(components[i]);
  components=(char **) RelinquishMagickMemory(components);
  return(node);
}
