MagickExport XMLTreeInfo *InsertTagIntoXMLTree(XMLTreeInfo *xml_info,
  XMLTreeInfo *child,const size_t offset)
{
  XMLTreeInfo
    *head,
    *node,
    *previous;

  child->ordered=(XMLTreeInfo *) NULL;
  child->sibling=(XMLTreeInfo *) NULL;
  child->next=(XMLTreeInfo *) NULL;
  child->offset=offset;
  child->parent=xml_info;
  if (xml_info->child == (XMLTreeInfo *) NULL)
    {
      xml_info->child=child;
      return(child);
    }
  head=xml_info->child;
  if (head->offset > offset)
    {
      child->ordered=head;
      xml_info->child=child;
    }
  else
    {
      node=head;
      while ((node->ordered != (XMLTreeInfo *) NULL) &&
             (node->ordered->offset <= offset))
        node=node->ordered;
      child->ordered=node->ordered;
      node->ordered=child;
    }
  previous=(XMLTreeInfo *) NULL;
  node=head;
  while ((node != (XMLTreeInfo *) NULL) && (strcmp(node->tag,child->tag) != 0))
  {
    previous=node;
    node=node->sibling;
  }
  if ((node != (XMLTreeInfo *) NULL) && (node->offset <= offset))
    {
      while ((node->next != (XMLTreeInfo *) NULL) &&
             (node->next->offset <= offset))
        node=node->next;
      child->next=node->next;
      node->next=child;
    }
  else
    {
      if ((previous != (XMLTreeInfo *) NULL) && (node != (XMLTreeInfo *) NULL))
        previous->sibling=node->sibling;
      child->next=node;
      previous=(XMLTreeInfo *) NULL;
      node=head;
      while ((node != (XMLTreeInfo *) NULL) && (node->offset <= offset))
      {
        previous=node;
        node=node->sibling;
      }
      child->sibling=node;
      if (previous != (XMLTreeInfo *) NULL)
        previous->sibling=child;
    }
  return(child);
}
