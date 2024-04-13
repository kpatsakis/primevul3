export_pamenv (void)
{
  char **env;

  /* This is a copy but don't care to free as we exec later anyways.  */
  env = pam_getenvlist (pamh);
  while (env && *env)
    {
      if (putenv (*env) != 0)
	err (EXIT_FAILURE, NULL);
      env++;
    }
}
