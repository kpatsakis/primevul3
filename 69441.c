change_identity (const struct passwd *pw)
{
  if (setgid (pw->pw_gid))
    err (EXIT_FAILURE,  _("cannot set group id"));
  if (setuid (pw->pw_uid))
    err (EXIT_FAILURE,  _("cannot set user id"));
}
