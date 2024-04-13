authenticate (const struct passwd *pw)
{
  const struct passwd *lpw = NULL;
  const char *cp, *srvname = NULL;
  int retval;

  switch (su_mode) {
  case SU_MODE:
    srvname = simulate_login ? PAM_SRVNAME_SU_L : PAM_SRVNAME_SU;
    break;
  case RUNUSER_MODE:
    srvname = simulate_login ? PAM_SRVNAME_RUNUSER_L : PAM_SRVNAME_RUNUSER;
    break;
  default:
    abort();
    break;
  }

  retval = pam_start (srvname, pw->pw_name, &conv, &pamh);
  if (is_pam_failure(retval))
    goto done;

  if (isatty (0) && (cp = ttyname (0)) != NULL)
    {
      const char *tty;

      if (strncmp (cp, "/dev/", 5) == 0)
	tty = cp + 5;
      else
	tty = cp;
      retval = pam_set_item (pamh, PAM_TTY, tty);
      if (is_pam_failure(retval))
	goto done;
    }

  lpw = current_getpwuid ();
  if (lpw && lpw->pw_name)
    {
      retval = pam_set_item (pamh, PAM_RUSER, (const void *) lpw->pw_name);
      if (is_pam_failure(retval))
	goto done;
    }

  if (su_mode == RUNUSER_MODE)
    {
      /*
       * This is the only difference between runuser(1) and su(1). The command
       * runuser(1) does not required authentication, because user is root.
       */
      if (restricted)
	errx(EXIT_FAILURE, _("may not be used by non-root users"));
      return;
    }

  retval = pam_authenticate (pamh, 0);
  if (is_pam_failure(retval))
    goto done;

  retval = pam_acct_mgmt (pamh, 0);
  if (retval == PAM_NEW_AUTHTOK_REQD)
    {
      /* Password has expired.  Offer option to change it.  */
      retval = pam_chauthtok (pamh, PAM_CHANGE_EXPIRED_AUTHTOK);
    }

done:

  log_syslog(pw, !is_pam_failure(retval));

  if (is_pam_failure(retval))
    {
      const char *msg;

      log_btmp(pw);

      msg  = pam_strerror(pamh, retval);
      pam_end(pamh, retval);
      sleep (getlogindefs_num ("FAIL_DELAY", 1));
      errx (EXIT_FAILURE, "%s", msg?msg:_("incorrect password"));
    }
}
