static void DestroyXMLTreeOrdered(XMLTreeInfo *xml_info)
{
  XMLTreeInfo
    *node,
    *ordered;

  ordered=xml_info->ordered;
  while(ordered != (XMLTreeInfo *) NULL)
  {
    node=ordered;
    ordered=node->ordered;
    node->ordered=(XMLTreeInfo *) NULL;
    (void) DestroyXMLTree(node);
  }
}
