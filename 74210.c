MODRET auth_pre_pass(cmd_rec *cmd) {
  char *displaylogin;

  pr_auth_endpwent(cmd->tmp_pool);
  pr_auth_endgrent(cmd->tmp_pool);

  /* Look for a DisplayLogin file which has an absolute path.  If we find one,
   * open a filehandle, such that that file can be displayed even if the
   * session is chrooted.  DisplayLogin files with relative paths will be
   * handled after chroot, preserving the old behavior.
   */

  displaylogin = get_param_ptr(TOPLEVEL_CONF, "DisplayLogin", FALSE);
  if (displaylogin &&
      *displaylogin == '/') {
    struct stat st;

    displaylogin_fh = pr_fsio_open(displaylogin, O_RDONLY);
    if (displaylogin_fh == NULL) {
      pr_log_debug(DEBUG6, "unable to open DisplayLogin file '%s': %s",
        displaylogin, strerror(errno));

    } else {
      if (pr_fsio_fstat(displaylogin_fh, &st) < 0) {
        pr_log_debug(DEBUG6, "unable to stat DisplayLogin file '%s': %s",
          displaylogin, strerror(errno));
        pr_fsio_close(displaylogin_fh);
        displaylogin_fh = NULL;

      } else {
        if (S_ISDIR(st.st_mode)) {
          errno = EISDIR;
          pr_log_debug(DEBUG6, "unable to use DisplayLogin file '%s': %s",
            displaylogin, strerror(errno));
          pr_fsio_close(displaylogin_fh);
          displaylogin_fh = NULL;
        }
      }
    }
  }

  return PR_DECLINED(cmd);
}
