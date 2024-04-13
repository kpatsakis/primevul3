static void ParseProcessingInstructions(XMLTreeRoot *root,char *xml,
  size_t length)
{
  char
    *target;

  register ssize_t
    i;

  ssize_t
    j;

  target=xml;
  xml[length]='\0';
  xml+=strcspn(xml,XMLWhitespace);
  if (*xml != '\0')
    {
      *xml='\0';
      xml+=strspn(xml+1,XMLWhitespace)+1;
    }
  if (strcmp(target,"xml") == 0)
    {
      xml=strstr(xml,"standalone");
      if ((xml != (char *) NULL) &&
          (strncmp(xml+strspn(xml+10,XMLWhitespace "='\"")+10,"yes",3) == 0))
        root->standalone=MagickTrue;
      return;
    }
  if (root->processing_instructions[0] == (char **) NULL)
    {
      root->processing_instructions=(char ***) AcquireMagickMemory(sizeof(
        *root->processing_instructions));
      if (root->processing_instructions ==(char ***) NULL)
        ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
      *root->processing_instructions=(char **) NULL;
    }
  i=0;
  while ((root->processing_instructions[i] != (char **) NULL) &&
         (strcmp(target,root->processing_instructions[i][0]) != 0))
    i++;
  if (root->processing_instructions[i] == (char **) NULL)
    {
      root->processing_instructions=(char ***) ResizeQuantumMemory(
        root->processing_instructions,(size_t) (i+2),
        sizeof(*root->processing_instructions));
      if (root->processing_instructions == (char ***) NULL)
        ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
      root->processing_instructions[i]=(char **) AcquireQuantumMemory(3,
        sizeof(**root->processing_instructions));
      if (root->processing_instructions[i] == (char **) NULL)
        ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
      root->processing_instructions[i+1]=(char **) NULL;
      root->processing_instructions[i][0]=ConstantString(target);
      root->processing_instructions[i][1]=(char *)
        root->processing_instructions[i+1];
      root->processing_instructions[i+1]=(char **) NULL;
      root->processing_instructions[i][2]=ConstantString("");
    }
  j=1;
  while (root->processing_instructions[i][j] != (char *) NULL)
    j++;
  root->processing_instructions[i]=(char **) ResizeQuantumMemory(
    root->processing_instructions[i],(size_t) (j+3),
    sizeof(**root->processing_instructions));
  if (root->processing_instructions[i] == (char **) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  root->processing_instructions[i][j+2]=(char *) ResizeQuantumMemory(
    root->processing_instructions[i][j+1],(size_t) (j+1),
    sizeof(***root->processing_instructions));
  if (root->processing_instructions[i][j+2] == (char *) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  (void) CopyMagickString(root->processing_instructions[i][j+2]+j-1,
    root->root.tag != (char *) NULL ? ">" : "<",2);
  root->processing_instructions[i][j]=ConstantString(xml);
  root->processing_instructions[i][j+1]=(char *) NULL;
}
