static void ParseCharacterContent(XMLTreeRoot *root,char *xml,
  const size_t length,const char state)
{
  XMLTreeInfo
    *xml_info;

  xml_info=root->node;
  if ((xml_info == (XMLTreeInfo *) NULL) || (xml_info->tag == (char *) NULL) ||
      (length == 0))
    return;
  xml[length]='\0';
  xml=ParseEntities(xml,root->entities,state);
  if ((xml_info->content != (char *) NULL) && (*xml_info->content != '\0'))
    {
      (void) ConcatenateString(&xml_info->content,xml);
      xml=DestroyString(xml);
    }
  else
    {
      if (xml_info->content != (char *) NULL)
        xml_info->content=DestroyString(xml_info->content);
      xml_info->content=xml;
    }
}
