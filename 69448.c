set_path(const struct passwd* pw)
{
  int r;
  if (pw->pw_uid)
    r = logindefs_setenv("PATH", "ENV_PATH", _PATH_DEFPATH);

  else if ((r = logindefs_setenv("PATH", "ENV_ROOTPATH", NULL)) != 0)
    r = logindefs_setenv("PATH", "ENV_SUPATH", _PATH_DEFPATH_ROOT);

  if (r != 0)
    err (EXIT_FAILURE,  _("failed to set the %s environment variable"), "PATH");
}
