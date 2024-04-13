MODRET set_anonrejectpasswords(cmd_rec *cmd) {
#ifdef PR_USE_REGEX
  pr_regex_t *pre = NULL;
  int res;

  CHECK_ARGS(cmd, 1);
  CHECK_CONF(cmd, CONF_ANON);

  pre = pr_regexp_alloc(&auth_module);

  res = pr_regexp_compile(pre, cmd->argv[1], REG_EXTENDED|REG_NOSUB);
  if (res != 0) {
    char errstr[200] = {'\0'};

    pr_regexp_error(res, pre, errstr, 200);
    pr_regexp_free(NULL, pre);

    CONF_ERROR(cmd, pstrcat(cmd->tmp_pool, "Unable to compile regex '",
      cmd->argv[1], "': ", errstr, NULL));
  }

  (void) add_config_param(cmd->argv[0], 1, (void *) pre);
  return PR_HANDLED(cmd);

#else
  CONF_ERROR(cmd, pstrcat(cmd->tmp_pool, "The ", cmd->argv[0], " directive "
    "cannot be used on this system, as you do not have POSIX compliant "
    "regex support", NULL));
#endif
}
