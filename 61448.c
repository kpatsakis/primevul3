 WandExport MagickBooleanType MagickImageCommand(ImageInfo *image_info,int argc,
  char **argv,char **metadata,ExceptionInfo *exception)
{
  MagickCLI
    *cli_wand;

  size_t
    len;

  assert(image_info != (ImageInfo *) NULL);

  /* For specific OS command line requirements */
  ReadCommandlLine(argc,&argv);

  /* Initialize special "CLI Wand" to hold images and settings (empty) */
  cli_wand=AcquireMagickCLI(image_info,exception);
  cli_wand->location="Initializing";
  cli_wand->filename=argv[0];
  cli_wand->line=1;

  if (cli_wand->wand.debug != MagickFalse)
    (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
         "\"%s\"",argv[0]);


  GetPathComponent(argv[0],TailPath,cli_wand->wand.name);
  SetClientName(cli_wand->wand.name);
  ConcatenateMagickString(cli_wand->wand.name,"-CLI",MagickPathExtent);

  len=strlen(argv[0]);  /* precaution */

  /* "convert" command - give a "deprecated" warning" */
  if (len>=7 && LocaleCompare("convert",argv[0]+len-7) == 0) {
    cli_wand->process_flags = ConvertCommandOptionFlags;
    (void) FormatLocaleFile(stderr,"WARNING: %s\n",
         "The convert command is deprecated in IMv7, use \"magick\"\n");
  }

  /* Special Case:  If command name ends with "script" implied "-script" */
  if (len>=6 && LocaleCompare("script",argv[0]+len-6) == 0) {
    if (argc >= 2 && (  (*(argv[1]) != '-') || (strlen(argv[1]) == 1) )) {
      GetPathComponent(argv[1],TailPath,cli_wand->wand.name);
      ProcessScriptOptions(cli_wand,argv[1],argc,argv,2);
      goto Magick_Command_Cleanup;
    }
  }

  /* Special Case: Version Information and Abort */
  if (argc == 2) {
    if ((LocaleCompare("-version",argv[1]) == 0)   || /* GNU standard option */
        (LocaleCompare("--version",argv[1]) == 0) ) { /* just version */
      CLIOption(cli_wand, "-version");
      goto Magick_Command_Exit;
    }
    if ((LocaleCompare("-help",argv[1]) == 0)   || /* GNU standard option */
        (LocaleCompare("--help",argv[1]) == 0) ) { /* just a brief summary */
      if (cli_wand->wand.debug != MagickFalse)
        (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
            "- Special Option \"%s\"", argv[1]);
      MagickUsage(MagickFalse);
      goto Magick_Command_Exit;
    }
    if (LocaleCompare("-usage",argv[1]) == 0) {   /* both version & usage */
      if (cli_wand->wand.debug != MagickFalse)
        (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
            "- Special Option \"%s\"", argv[1]);
      CLIOption(cli_wand, "-version" );
      MagickUsage(MagickTrue);
      goto Magick_Command_Exit;
    }
  }

  /* not enough arguments -- including -help */
  if (argc < 3) {
    (void) FormatLocaleFile(stderr,
       "Error: Invalid argument or not enough arguments\n\n");
    MagickUsage(MagickFalse);
    goto Magick_Command_Exit;
  }

  /* Special "concatenate option (hidden) for delegate usage */
  if (LocaleCompare("-concatenate",argv[1]) == 0) {
    if (cli_wand->wand.debug != MagickFalse)
        (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
            "- Special Option \"%s\"", argv[1]);
    ConcatenateImages(argc,argv,exception);
    goto Magick_Command_Exit;
  }

  /* List Information and Abort */
  if (argc == 3 && LocaleCompare("-list",argv[1]) == 0) {
    CLIOption(cli_wand, argv[1], argv[2]);
    goto Magick_Command_Exit;
  }

  /* ------------- */
  /* The Main Call */

  if (LocaleCompare("-script",argv[1]) == 0) {
    /* Start processing directly from script, no pre-script options
       Replace wand command name with script name
       First argument in the argv array is the script name to read.
    */
    GetPathComponent(argv[2],TailPath,cli_wand->wand.name);
    ProcessScriptOptions(cli_wand,argv[2],argc,argv,3);
  }
  else {
    /* Normal Command Line, assumes output file as last option */
    ProcessCommandOptions(cli_wand,argc,argv,1);
  }
  /* ------------- */

Magick_Command_Cleanup:
  cli_wand->location="Cleanup";
  cli_wand->filename=argv[0];
  if (cli_wand->wand.debug != MagickFalse)
    (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
         "\"%s\"",argv[0]);

  /* recover original image_info and clean up stacks
     FUTURE: "-reset stacks" option  */
  while ((cli_wand->image_list_stack != (Stack *) NULL) &&
         (cli_wand->image_list_stack->next != (Stack *) NULL))
    CLIOption(cli_wand,")");
  while ((cli_wand->image_info_stack != (Stack *) NULL) &&
         (cli_wand->image_info_stack->next != (Stack *) NULL))
    CLIOption(cli_wand,"}");
