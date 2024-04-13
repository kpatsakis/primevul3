WandExport int ProcessCommandOptions(MagickCLI *cli_wand,int argc,char **argv,
  int index)
{
  const char
    *option,
    *arg1,
    *arg2;

  int
    i,
    end,
    count;

  CommandOptionFlags
    option_type;

  assert(argc>=index); /* you may have no arguments left! */
  assert(argv != (char **) NULL);
  assert(argv[index] != (char *) NULL);
  assert(argv[argc-1] != (char *) NULL);
  assert(cli_wand != (MagickCLI *) NULL);
  assert(cli_wand->signature == MagickWandSignature);

  /* define the error location string for use in exceptions
     order of localtion format escapes: filename, line, column */
  cli_wand->location="at %s arg %u";
  cli_wand->filename="CLI";
  cli_wand->line=index;  /* note first argument we will process */

  if (cli_wand->wand.debug != MagickFalse)
    (void) CLILogEvent(cli_wand,CommandEvent,GetMagickModule(),
         "- Starting (\"%s\")", argv[index]);

  end = argc;
  if ( (cli_wand->process_flags & ProcessImplictWrite) != 0 )
    end--; /* the last arument is an implied write, do not process directly */

  for (i=index; i < end; i += count +1) {
    /* Finished processing one option? */
    if ( (cli_wand->process_flags & ProcessOneOptionOnly) != 0 && i != index )
      return(i);

    do { /* use break to loop to exception handler and loop */

      option=argv[i];
      cli_wand->line=i;  /* note the argument for this option */

      /* get option, its argument count, and option type */
      cli_wand->command = GetCommandOptionInfo(argv[i]);
      count=cli_wand->command->type;
      option_type=(CommandOptionFlags) cli_wand->command->flags;
#if 0
      (void) FormatLocaleFile(stderr, "CLI %d: \"%s\" matched \"%s\"\n",
            i, argv[i], cli_wand->command->mnemonic );
#endif

      if ( option_type == UndefinedOptionFlag ||
           (option_type & NonMagickOptionFlag) != 0 ) {
#if MagickCommandDebug >= 3
        (void) FormatLocaleFile(stderr, "CLI arg %d Non-Option: \"%s\"\n",
             i, option);
#endif
        if (IsCommandOption(option) == MagickFalse) {
          if ( (cli_wand->process_flags & ProcessImplictRead) != 0 ) {
            /* non-option -- treat as a image read */
            cli_wand->command=(const OptionInfo *) NULL;
            CLIOption(cli_wand,"-read",option);
            break; /* next option */
          }
        }
        CLIWandException(OptionFatalError,"UnrecognizedOption",option);
        break; /* next option */
      }

      if ( ((option_type & SpecialOptionFlag) != 0 ) &&
           ((cli_wand->process_flags & ProcessScriptOption) != 0) &&
           (LocaleCompare(option,"-script") == 0) ) {
        /* Call Script from CLI, with a filename as a zeroth argument.
           NOTE: -script may need to use the 'implict write filename' argument
           so it must be handled specially to prevent a 'missing argument' error.
        */
        if ( (i+count) >= argc )
          CLIWandException(OptionFatalError,"MissingArgument",option);
        ProcessScriptOptions(cli_wand,argv[i+1],argc,argv,i+count);
        return(argc);  /* Script does not return to CLI -- Yet */
                       /* FUTURE: when it does, their may be no write arg! */
      }

      if ((i+count) >= end ) {
        CLIWandException(OptionFatalError,"MissingArgument",option);
        if ( CLICatchException(cli_wand, MagickFalse) != MagickFalse )
          return(end);
        break; /* next option - not that their is any! */
      }

      arg1 = ( count >= 1 ) ? argv[i+1] : (char *) NULL;
      arg2 = ( count >= 2 ) ? argv[i+2] : (char *) NULL;

      /*
        Process Known Options
      */
#if MagickCommandDebug >= 3
      (void) FormatLocaleFile(stderr,
        "CLI arg %u Option: \"%s\"  Count: %d  Flags: %04x  Args: \"%s\" \"%s\"\n",
            i,option,count,option_type,arg1,arg2);
#endif
      /* ignore 'genesis options' in command line args */
      if ( (option_type & GenesisOptionFlag) != 0 )
        break; /* next option */

      /* Handle any special options for CLI (-script handled above) */
      if ( (option_type & SpecialOptionFlag) != 0 ) {
        if ( (cli_wand->process_flags & ProcessExitOption) != 0
             && LocaleCompare(option,"-exit") == 0 )
          return(i+count);
        break; /* next option */
      }

      /* Process standard image option */
      CLIOption(cli_wand, option, arg1, arg2);

DisableMSCWarning(4127)
    } while (0); /* break block to next option */
RestoreMSCWarning

#if MagickCommandDebug >= 5
    (void) FormatLocaleFile(stderr, "CLI-post Image Count = %ld\n",
         (long) GetImageListLength(cli_wand->wand.images) );
#endif
    if ( CLICatchException(cli_wand, MagickFalse) != MagickFalse )
      return(i+count);
  }
  assert(i==end);

  if ( (cli_wand->process_flags & ProcessImplictWrite) == 0 )
    return(end); /* no implied write -- just return to caller */

  assert(end==argc-1); /* end should not include last argument */

  /*
     Implicit Write of images to final CLI argument
  */
  option=argv[i];
  cli_wand->line=i;

  /* check that stacks are empty - or cause exception */
  if (cli_wand->image_list_stack != (Stack *) NULL)
    CLIWandException(OptionError,"UnbalancedParenthesis", "(end of cli)");
  else if (cli_wand->image_info_stack != (Stack *) NULL)
    CLIWandException(OptionError,"UnbalancedBraces", "(end of cli)");
  if ( CLICatchException(cli_wand, MagickFalse) != MagickFalse )
    return(argc);

#if MagickCommandDebug >= 3
  (void) FormatLocaleFile(stderr,"CLI arg %d Write File: \"%s\"\n",i,option);
#endif

  /* Valid 'do no write' replacement option (instead of "null:") */
  if (LocaleCompare(option,"-exit") == 0 )
    return(argc);  /* just exit, no image write */

  /* If filename looks like an option,
     Or the common 'end of line' error of a single space.
     -- produce an error */
  if (IsCommandOption(option) != MagickFalse ||
      (option[0] == ' ' && option[1] == '\0') ) {
    CLIWandException(OptionError,"MissingOutputFilename",option);
    return(argc);
  }

  cli_wand->command=(const OptionInfo *) NULL;
  CLIOption(cli_wand,"-write",option);
  return(argc);
}
