parse_args (int          *argcp,
            const char ***argvp)
{
  int total_parsed_argc = *argcp;

  parse_args_recurse (argcp, argvp, FALSE, &total_parsed_argc);
}
