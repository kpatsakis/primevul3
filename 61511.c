MagickExport XMLTreeInfo *PruneTagFromXMLTree(XMLTreeInfo *xml_info)
{
  XMLTreeInfo
    *node;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  if (xml_info->next != (XMLTreeInfo *) NULL)
    xml_info->next->sibling=xml_info->sibling;
  if (xml_info->parent != (XMLTreeInfo *) NULL)
    {
      node=xml_info->parent->child;
      if (node == xml_info)
        xml_info->parent->child=xml_info->ordered;
      else
        {
          while (node->ordered != xml_info)
            node=node->ordered;
          node->ordered=node->ordered->ordered;
          node=xml_info->parent->child;
          if (strcmp(node->tag,xml_info->tag) != 0)
            {
              while (strcmp(node->sibling->tag,xml_info->tag) != 0)
                node=node->sibling;
              if (node->sibling != xml_info)
                node=node->sibling;
              else
                node->sibling=(xml_info->next != (XMLTreeInfo *) NULL) ?
                  xml_info->next : node->sibling->sibling;
            }
          while ((node->next != (XMLTreeInfo *) NULL) &&
                 (node->next != xml_info))
            node=node->next;
          if (node->next != (XMLTreeInfo *) NULL)
            node->next=node->next->next;
        }
    }
  xml_info->ordered=(XMLTreeInfo *) NULL;
  xml_info->sibling=(XMLTreeInfo *) NULL;
  xml_info->next=(XMLTreeInfo *) NULL;
  return(xml_info);
}
