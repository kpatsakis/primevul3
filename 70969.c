handle_die_with_parent (void)
{
  if (opt_die_with_parent && prctl (PR_SET_PDEATHSIG, SIGKILL, 0, 0, 0) != 0)
    die_with_error ("prctl");
}
