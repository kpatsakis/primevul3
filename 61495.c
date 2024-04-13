MagickExport MagickBooleanType GetXMLTreeAttributes(const XMLTreeInfo *xml_info,
  SplayTreeInfo *attributes)
{
  register ssize_t
    i;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((const XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  assert(attributes != (SplayTreeInfo *) NULL);
  if (xml_info->attributes == (char **) NULL)
    return(MagickTrue);
  i=0;
  while (xml_info->attributes[i] != (char *) NULL)
  {
     (void) AddValueToSplayTree(attributes,
       ConstantString(xml_info->attributes[i]),
       ConstantString(xml_info->attributes[i+1]));
    i+=2;
  }
  return(MagickTrue);
}
