MagickExport XMLTreeInfo *NewXMLTreeTag(const char *tag)
{
  static const char
    *predefined_entities[NumberPredefinedEntities+1] =
    {
      "lt;", "&#60;", "gt;", "&#62;", "quot;", "&#34;",
      "apos;", "&#39;", "amp;", "&#38;", (char *) NULL
    };

  XMLTreeRoot
    *root;

  root=(XMLTreeRoot *) AcquireMagickMemory(sizeof(*root));
  if (root == (XMLTreeRoot *) NULL)
    return((XMLTreeInfo *) NULL);
  (void) ResetMagickMemory(root,0,sizeof(*root));
  root->root.tag=(char *) NULL;
  if (tag != (char *) NULL)
    root->root.tag=ConstantString(tag);
  root->node=(&root->root);
  root->root.content=ConstantString("");
  root->entities=(char **) AcquireMagickMemory(sizeof(predefined_entities));
  if (root->entities == (char **) NULL)
    return((XMLTreeInfo *) NULL);
  (void) CopyMagickMemory(root->entities,predefined_entities,
    sizeof(predefined_entities));
  root->root.attributes=sentinel;
  root->attributes=(char ***) root->root.attributes;
  root->processing_instructions=(char ***) root->root.attributes;
  root->debug=IsEventLogging();
  root->signature=MagickSignature;
  return(&root->root);
}
