MagickExport MagickBooleanType OpenModules(ExceptionInfo *exception)
{
  char
    **modules;

  register ssize_t
    i;

  size_t
    number_modules;

  /*
    Load all modules.
  */
  (void) GetMagickInfo((char *) NULL,exception);
  number_modules=0;
  modules=GetModuleList("*",MagickImageCoderModule,&number_modules,exception);
  if ((modules == (char **) NULL) || (*modules == (char *) NULL))
    {
      if (modules != (char **) NULL)
        modules=(char **) RelinquishMagickMemory(modules);
      return(MagickFalse);
    }
  for (i=0; i < (ssize_t) number_modules; i++)
    (void) OpenModule(modules[i],exception);
  /*
    Relinquish resources.
  */
  for (i=0; i < (ssize_t) number_modules; i++)
    modules[i]=DestroyString(modules[i]);
  modules=(char **) RelinquishMagickMemory(modules);
  return(MagickTrue);
}
