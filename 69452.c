usage (int status)
{
  if (su_mode == RUNUSER_MODE) {
    fputs(USAGE_HEADER, stdout);
    printf (_(" %s [options] -u <user> [[--] <command>]\n"), program_invocation_short_name);
    printf (_(" %s [options] [-] [<user> [<argument>...]]\n"), program_invocation_short_name);
    fputs (_("\n"
    "Run <command> with the effective user ID and group ID of <user>.  If -u is\n"
    "not given, fall back to su(1)-compatible semantics and execute standard shell.\n"
    "The options -c, -f, -l, and -s are mutually exclusive with -u.\n"), stdout);

    fputs(USAGE_OPTIONS, stdout);

    fputs (_(" -u, --user <user>             username\n"), stdout);

  } else {
    fputs(USAGE_HEADER, stdout);
    printf (_(" %s [options] [-] [<user> [<argument>...]]\n"), program_invocation_short_name);
    fputs (_("\n"
    "Change the effective user ID and group ID to that of <user>.\n"
    "A mere - implies -l.  If <user> is not given, root is assumed.\n"), stdout);

    fputs(USAGE_OPTIONS, stdout);
  }

  fputs (_(" -m, -p, --preserve-environment  do not reset environment variables\n"), stdout);
  fputs (_(" -g, --group <group>             specify the primary group\n"), stdout);
  fputs (_(" -G, --supp-group <group>        specify a supplemental group\n\n"), stdout);

  fputs (_(" -, -l, --login                  make the shell a login shell\n"), stdout);
  fputs (_(" -c, --command <command>         pass a single command to the shell with -c\n"), stdout);
  fputs (_(" --session-command <command>     pass a single command to the shell with -c\n"
           "                                   and do not create a new session\n"), stdout);
  fputs (_(" -f, --fast                      pass -f to the shell (for csh or tcsh)\n"), stdout);
  fputs (_(" -s, --shell <shell>             run <shell> if /etc/shells allows it\n"), stdout);

  fputs(USAGE_SEPARATOR, stdout);
  fputs(USAGE_HELP, stdout);
  fputs(USAGE_VERSION, stdout);
  printf(USAGE_MAN_TAIL(su_mode == SU_MODE ? "su(1)" : "runuser(1)"));
  exit (status);
}
