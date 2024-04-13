su_main (int argc, char **argv, int mode)
{
  int optc;
  const char *new_user = DEFAULT_USER, *runuser_user = NULL;
  char *command = NULL;
  int request_same_session = 0;
  char *shell = NULL;
  struct passwd *pw;
  struct passwd pw_copy;

  gid_t *groups = NULL;
  size_t ngroups = 0;
  bool use_supp = false;
  bool use_gid = false;
  gid_t gid = 0;

  static const struct option longopts[] = {
    {"command", required_argument, NULL, 'c'},
    {"session-command", required_argument, NULL, 'C'},
    {"fast", no_argument, NULL, 'f'},
    {"login", no_argument, NULL, 'l'},
    {"preserve-environment", no_argument, NULL, 'p'},
    {"shell", required_argument, NULL, 's'},
    {"group", required_argument, NULL, 'g'},
    {"supp-group", required_argument, NULL, 'G'},
    {"user", required_argument, NULL, 'u'},		/* runuser only */
    {"help", no_argument, 0, 'h'},
    {"version", no_argument, 0, 'V'},
    {NULL, 0, NULL, 0}
  };

  setlocale (LC_ALL, "");
  bindtextdomain (PACKAGE, LOCALEDIR);
  textdomain (PACKAGE);
  atexit(close_stdout);

  su_mode = mode;
  fast_startup = false;
  simulate_login = false;
  change_environment = true;

  while ((optc = getopt_long (argc, argv, "c:fg:G:lmps:u:hV", longopts, NULL)) != -1)
    {
      switch (optc)
	{
	case 'c':
	  command = optarg;
	  break;

        case 'C':
          command = optarg;
          request_same_session = 1;
          break;

	case 'f':
	  fast_startup = true;
	  break;

	case 'g':
	  use_gid = true;
	  gid = add_supp_group(optarg, &groups, &ngroups);
	  break;

	case 'G':
	  use_supp = true;
	  add_supp_group(optarg, &groups, &ngroups);
	  break;

	case 'l':
	  simulate_login = true;
	  break;

	case 'm':
	case 'p':
	  change_environment = false;
	  break;

	case 's':
	  shell = optarg;
	  break;

	case 'u':
	  if (su_mode != RUNUSER_MODE)
	    usage (EXIT_FAILURE);
	  runuser_user = optarg;
	  break;

	case 'h':
	  usage(0);

	case 'V':
	  printf(UTIL_LINUX_VERSION);
	  exit(EXIT_SUCCESS);

	default:
	  errtryhelp(EXIT_FAILURE);
	}
    }

  restricted = evaluate_uid ();

  if (optind < argc && !strcmp (argv[optind], "-"))
    {
      simulate_login = true;
      ++optind;
    }

  if (simulate_login && !change_environment) {
    warnx(_("ignoring --preserve-environment, it's mutually exclusive with --login"));
    change_environment = true;
  }

  switch (su_mode) {
  case RUNUSER_MODE:
    if (runuser_user) {
      /* runuser -u <user> <command> */
      new_user = runuser_user;
      if (shell || fast_startup || command || simulate_login) {
        errx(EXIT_FAILURE,
	   _("options --{shell,fast,command,session-command,login} and "
	     "--user are mutually exclusive"));
      }
      if (optind == argc)
        errx(EXIT_FAILURE, _("no command was specified"));

      break;
    }
    /* fallthrough if -u <user> is not specified, then follow
     * traditional su(1) behavior
     */
  case SU_MODE:
    if (optind < argc)
      new_user = argv[optind++];
    break;
  }

  if ((use_supp || use_gid) && restricted)
    errx(EXIT_FAILURE, _("only root can specify alternative groups"));

  logindefs_load_defaults = load_config;

  pw = getpwnam (new_user);
  if (! (pw && pw->pw_name && pw->pw_name[0] && pw->pw_dir && pw->pw_dir[0]
	 && pw->pw_passwd))
    errx (EXIT_FAILURE, _("user %s does not exist"), new_user);

  /* Make a copy of the password information and point pw at the local
     copy instead.  Otherwise, some systems (e.g. Linux) would clobber
     the static data through the getlogin call from log_su.
     Also, make sure pw->pw_shell is a nonempty string.
     It may be NULL when NEW_USER is a username that is retrieved via NIS (YP),
     but that doesn't have a default shell listed.  */
  pw_copy = *pw;
  pw = &pw_copy;
  pw->pw_name = xstrdup (pw->pw_name);
  pw->pw_passwd = xstrdup (pw->pw_passwd);
  pw->pw_dir = xstrdup (pw->pw_dir);
  pw->pw_shell = xstrdup (pw->pw_shell && pw->pw_shell[0]
			  ? pw->pw_shell
			  : DEFAULT_SHELL);
  endpwent ();

  if (use_supp && !use_gid)
    pw->pw_gid = groups[0];
  else if (use_gid)
    pw->pw_gid = gid;

  authenticate (pw);

  if (request_same_session || !command || !pw->pw_uid)
    same_session = 1;

  /* initialize shell variable only if "-u <user>" not specified */
  if (runuser_user) {
    shell = NULL;
  } else {
    if (!shell && !change_environment)
      shell = getenv ("SHELL");
    if (shell && getuid () != 0 && restricted_shell (pw->pw_shell))
      {
	/* The user being su'd to has a nonstandard shell, and so is
	   probably a uucp account or has restricted access.  Don't
	   compromise the account by allowing access with a standard
	   shell.  */
	warnx (_("using restricted shell %s"), pw->pw_shell);
	shell = NULL;
      }
    shell = xstrdup (shell ? shell : pw->pw_shell);
  }

  init_groups (pw, groups, ngroups);

  if (!simulate_login || command)
    suppress_pam_info = 1;		/* don't print PAM info messages */

  create_watching_parent ();
  /* Now we're in the child.  */

  change_identity (pw);
  if (!same_session)
    setsid ();

  /* Set environment after pam_open_session, which may put KRB5CCNAME
     into the pam_env, etc.  */

  modify_environment (pw, shell);

  if (simulate_login && chdir (pw->pw_dir) != 0)
    warn (_("warning: cannot change directory to %s"), pw->pw_dir);

  if (shell)
    run_shell (shell, command, argv + optind, max (0, argc - optind));
  else {
    execvp(argv[optind], &argv[optind]);
    err(EXIT_FAILURE, _("failed to execute %s"), argv[optind]);
  }
}
