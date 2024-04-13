propagate_exit_status (int status)
{
  if (WIFEXITED (status))
    return WEXITSTATUS (status);

  /* The process died of a signal, we can't really report that, but we
   * can at least be bash-compatible. The bash manpage says:
   *   The return value of a simple command is its
   *   exit status, or 128+n if the command is
   *   terminated by signal n.
   */
  if (WIFSIGNALED (status))
    return 128 + WTERMSIG (status);

  /* Weird? */
  return 255;
}
