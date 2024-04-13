static void DestroyXMLTreeChild(XMLTreeInfo *xml_info)
{
  XMLTreeInfo
    *child,
    *node;

  child=xml_info->child;
  while(child != (XMLTreeInfo *) NULL)
  {
    node=child;
    child=node->child;
    node->child=(XMLTreeInfo *) NULL;
    (void) DestroyXMLTree(node);
  }
}
