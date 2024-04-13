MagickExport MagickBooleanType ListModuleInfo(FILE *file,
  ExceptionInfo *exception)
{
  char
    filename[MaxTextExtent],
    module_path[MaxTextExtent],
    **modules,
    path[MaxTextExtent];

  register ssize_t
    i;

  size_t
    number_modules;

  if (file == (const FILE *) NULL)
    file=stdout;
  /*
    List image coders.
  */
  modules=GetModuleList("*",MagickImageCoderModule,&number_modules,exception);
  if (modules == (char **) NULL)
    return(MagickFalse);
  TagToCoderModuleName("magick",filename);
  (void) GetMagickModulePath(filename,MagickImageCoderModule,module_path,
    exception);
  GetPathComponent(module_path,HeadPath,path);
  (void) FormatLocaleFile(file,"\nPath: %s\n\n",path);
  (void) FormatLocaleFile(file,"Image Coder\n");
  (void) FormatLocaleFile(file,
    "-------------------------------------------------"
    "------------------------------\n");
  for (i=0; i < (ssize_t) number_modules; i++)
  {
    (void) FormatLocaleFile(file,"%s",modules[i]);
    (void) FormatLocaleFile(file,"\n");
  }
  (void) fflush(file);
  /*
    Relinquish resources.
  */
  for (i=0; i < (ssize_t) number_modules; i++)
    modules[i]=DestroyString(modules[i]);
  modules=(char **) RelinquishMagickMemory(modules);
  /*
    List image filters.
  */
  modules=GetModuleList("*",MagickImageFilterModule,&number_modules,exception);
  if (modules == (char **) NULL)
    return(MagickFalse);
  TagToFilterModuleName("analyze",filename);
  (void) GetMagickModulePath(filename,MagickImageFilterModule,module_path,
    exception);
  GetPathComponent(module_path,HeadPath,path);
  (void) FormatLocaleFile(file,"\nPath: %s\n\n",path);
  (void) FormatLocaleFile(file,"Image Filter\n");
  (void) FormatLocaleFile(file,
    "-------------------------------------------------"
    "------------------------------\n");
  for (i=0; i < (ssize_t) number_modules; i++)
  {
    (void) FormatLocaleFile(file,"%s",modules[i]);
    (void) FormatLocaleFile(file,"\n");
  }
  (void) fflush(file);
  /*
    Relinquish resources.
  */
  for (i=0; i < (ssize_t) number_modules; i++)
    modules[i]=DestroyString(modules[i]);
  modules=(char **) RelinquishMagickMemory(modules);
  return(MagickTrue);
}
