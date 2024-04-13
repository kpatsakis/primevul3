MagickExport char *XMLTreeInfoToXML(XMLTreeInfo *xml_info)
{
  char
    *xml;

  register char
    *p,
    *q;

  register ssize_t
    i;

  size_t
    extent,
    length;

  ssize_t
    j,
    k;

  XMLTreeInfo
    *ordered,
    *parent;

  XMLTreeRoot
    *root;

  assert(xml_info != (XMLTreeInfo *) NULL);
  assert((xml_info->signature == MagickSignature) ||
         (((XMLTreeRoot *) xml_info)->signature == MagickSignature));
  if (xml_info->debug != MagickFalse)
    (void) LogMagickEvent(TraceEvent,GetMagickModule(),"...");
  if (xml_info->tag == (char *) NULL)
    return((char *) NULL);
  xml=AcquireString((char *) NULL);
  length=0;
  extent=MaxTextExtent;
  root=(XMLTreeRoot *) xml_info;
  while (root->root.parent != (XMLTreeInfo *) NULL)
    root=(XMLTreeRoot *) root->root.parent;
  parent=xml_info->parent;
  if (parent == (XMLTreeInfo *) NULL)
    for (i=0; root->processing_instructions[i] != (char **) NULL; i++)
    {
      /*
        Pre-root processing instructions.
      */
      for (k=2; root->processing_instructions[i][k-1]; k++) ;
      p=root->processing_instructions[i][1];
      for (j=1; p != (char *) NULL; j++)
      {
        if (root->processing_instructions[i][k][j-1] == '>')
          {
            p=root->processing_instructions[i][j];
            continue;
          }
        q=root->processing_instructions[i][0];
        if ((length+strlen(p)+strlen(q)+MaxTextExtent) > extent)
          {
            extent=length+strlen(p)+strlen(q)+MaxTextExtent;
            xml=(char *) ResizeQuantumMemory(xml,extent,sizeof(*xml));
            if (xml == (char *) NULL)
              return(xml);
          }
        length+=FormatLocaleString(xml+length,extent,"<?%s%s%s?>\n",q,
          *p != '\0' ? " " : "",p);
        p=root->processing_instructions[i][j];
      }
    }
  ordered=xml_info->ordered;
  xml_info->parent=(XMLTreeInfo *) NULL;
  xml_info->ordered=(XMLTreeInfo *) NULL;
  xml=XMLTreeTagToXML(xml_info,&xml,&length,&extent,0,root->attributes);
  xml_info->parent=parent;
  xml_info->ordered=ordered;
  if (parent == (XMLTreeInfo *) NULL)
    for (i=0; root->processing_instructions[i] != (char **) NULL; i++)
    {
      /*
        Post-root processing instructions.
      */
      for (k=2; root->processing_instructions[i][k-1]; k++) ;
      p=root->processing_instructions[i][1];
      for (j=1; p != (char *) NULL; j++)
      {
        if (root->processing_instructions[i][k][j-1] == '<')
          {
            p=root->processing_instructions[i][j];
            continue;
          }
        q=root->processing_instructions[i][0];
        if ((length+strlen(p)+strlen(q)+MaxTextExtent) > extent)
          {
            extent=length+strlen(p)+strlen(q)+MaxTextExtent;
            xml=(char *) ResizeQuantumMemory(xml,extent,sizeof(*xml));
            if (xml == (char *) NULL)
              return(xml);
          }
        length+=FormatLocaleString(xml+length,extent,"\n<?%s%s%s?>",q,
          *p != '\0' ? " " : "",p);
        p=root->processing_instructions[i][j];
      }
    }
  return((char *) ResizeQuantumMemory(xml,length+1,sizeof(*xml)));
}
