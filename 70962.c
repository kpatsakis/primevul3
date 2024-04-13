block_sigchild (void)
{
  sigset_t mask;
  int status;

  sigemptyset (&mask);
  sigaddset (&mask, SIGCHLD);

  if (sigprocmask (SIG_BLOCK, &mask, NULL) == -1)
    die_with_error ("sigprocmask");

  /* Reap any outstanding zombies that we may have inherited */
  while (waitpid (-1, &status, WNOHANG) > 0)
    ;
}
