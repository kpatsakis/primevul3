MagickExport XMLTreeInfo *SetXMLTreeAttribute(XMLTreeInfo *xml_info,
  const char *tag,const char *value)
{
  register ssize_t
    i;

  ssize_t
    j;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  i=0;
  while ((xml_info->attributes[i] != (char *) NULL) &&
         (strcmp(xml_info->attributes[i],tag) != 0))
    i+=2;
  if (xml_info->attributes[i] == (char *) NULL)
    {
      /*
        Add new attribute tag.
      */
      if (value == (const char *) NULL)
        return(xml_info);
      if (xml_info->attributes != sentinel)
        xml_info->attributes=(char **) ResizeQuantumMemory(
          xml_info->attributes,(size_t) (i+4),sizeof(*xml_info->attributes));
      else
        {
          xml_info->attributes=(char **) AcquireQuantumMemory(4,
            sizeof(*xml_info->attributes));
          if (xml_info->attributes != (char **) NULL)
            xml_info->attributes[1]=ConstantString("");
        }
      if (xml_info->attributes == (char **) NULL)
        ThrowFatalException(ResourceLimitFatalError,"UnableToAcquireString");
      xml_info->attributes[i]=ConstantString(tag);
      xml_info->attributes[i+2]=(char *) NULL;
      (void) strlen(xml_info->attributes[i+1]);
    }
  /*
    Add new value to an existing attribute.
  */
  for (j=i; xml_info->attributes[j] != (char *) NULL; j+=2) ;
  if (xml_info->attributes[i+1] != (char *) NULL)
    xml_info->attributes[i+1]=DestroyString(xml_info->attributes[i+1]);
  if (value != (const char *) NULL)
    {
      xml_info->attributes[i+1]=ConstantString(value);
      return(xml_info);
    }
  if (xml_info->attributes[i] != (char *) NULL)
    xml_info->attributes[i]=DestroyString(xml_info->attributes[i]);
  (void) CopyMagickMemory(xml_info->attributes+i,xml_info->attributes+i+2,
    (size_t) (j-i)*sizeof(*xml_info->attributes));
  xml_info->attributes=(char **) ResizeQuantumMemory(xml_info->attributes,
    (size_t) (j+2),sizeof(*xml_info->attributes));
  if (xml_info->attributes == (char **) NULL)
    ThrowFatalException(ResourceLimitFatalError,"UnableToAcquireString");
  j-=2;
  (void) CopyMagickMemory(xml_info->attributes[j+1]+(i/2),
    xml_info->attributes[j+1]+(i/2)+1,(size_t) (((j+2)/2)-(i/2))*
    sizeof(**xml_info->attributes));
  return(xml_info);
}
