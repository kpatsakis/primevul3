MagickExport char **GetModuleList(const char *pattern,
  const MagickModuleType type,size_t *number_modules,ExceptionInfo *exception)
{
#define MaxModules  511

  char
    **modules,
    filename[MaxTextExtent],
    module_path[MaxTextExtent],
    path[MaxTextExtent];

  DIR
    *directory;

  MagickBooleanType
    status;

  register ssize_t
    i;

  size_t
    max_entries;

  struct dirent
    *buffer,
    *entry;

  /*
    Locate all modules in the image coder or filter path.
  */
  switch (type)
  {
    case MagickImageCoderModule:
    default:
    {
      TagToCoderModuleName("magick",filename);
      status=GetMagickModulePath(filename,MagickImageCoderModule,module_path,
        exception);
      break;
    }
    case MagickImageFilterModule:
    {
      TagToFilterModuleName("analyze",filename);
      status=GetMagickModulePath(filename,MagickImageFilterModule,module_path,
        exception);
      break;
    }
  }
  if (status == MagickFalse)
    return((char **) NULL);
  GetPathComponent(module_path,HeadPath,path);
  max_entries=MaxModules;
  modules=(char **) AcquireQuantumMemory((size_t) max_entries+1UL,
    sizeof(*modules));
  if (modules == (char **) NULL)
    return((char **) NULL);
  *modules=(char *) NULL;
  directory=opendir(path);
  if (directory == (DIR *) NULL)
    {
      modules=(char **) RelinquishMagickMemory(modules);
      return((char **) NULL);
    }
  buffer=(struct dirent *) AcquireMagickMemory(sizeof(*buffer)+FILENAME_MAX+1);
  if (buffer == (struct dirent *) NULL)
    ThrowFatalException(ResourceLimitFatalError,"MemoryAllocationFailed");
  i=0;
  while ((MagickReadDirectory(directory,buffer,&entry) == 0) &&
         (entry != (struct dirent *) NULL))
  {
    status=GlobExpression(entry->d_name,ModuleGlobExpression,MagickFalse);
    if (status == MagickFalse)
      continue;
    if (GlobExpression(entry->d_name,pattern,MagickFalse) == MagickFalse)
      continue;
    if (i >= (ssize_t) max_entries)
      {
        modules=(char **) NULL;
        if (~max_entries > max_entries)
          modules=(char **) ResizeQuantumMemory(modules,(size_t)
            (max_entries << 1),sizeof(*modules));
        max_entries<<=1;
        if (modules == (char **) NULL)
          break;
      }
    /*
      Add new module name to list.
    */
    modules[i]=AcquireString((char *) NULL);
    GetPathComponent(entry->d_name,BasePath,modules[i]);
    if (LocaleNCompare("IM_MOD_",modules[i],7) == 0)
      {
        (void) CopyMagickString(modules[i],modules[i]+10,MaxTextExtent);
        modules[i][strlen(modules[i])-1]='\0';
      }
    i++;
  }
  buffer=(struct dirent *) RelinquishMagickMemory(buffer);
  (void) closedir(directory);
  if (modules == (char **) NULL)
    {
      (void) ThrowMagickException(exception,GetMagickModule(),ConfigureError,
        "MemoryAllocationFailed","`%s'",pattern);
      return((char **) NULL);
    }
  qsort((void *) modules,(size_t) i,sizeof(*modules),ModuleCompare);
  modules[i]=(char *) NULL;
  *number_modules=(size_t) i;
  return(modules);
}
