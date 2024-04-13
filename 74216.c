static char *get_default_chdir(pool *p, xaset_t *conf) {
  config_rec *c;
  char *dir = NULL;
  int ret;

  c = find_config(conf, CONF_PARAM, "DefaultChdir", FALSE);

  while (c) {
    /* Check the groups acl */
    if (c->argc < 2) {
      dir = c->argv[0];
      break;
    }

    ret = pr_expr_eval_group_and(((char **) c->argv)+1);

    if (ret) {
      dir = c->argv[0];
      break;
    }

    c = find_config_next(c, c->next, CONF_PARAM, "DefaultChdir", FALSE);
  }

  /* If the directory is relative, concatenate w/ session.cwd. */
  if (dir && *dir != '/' && *dir != '~')
    dir = pdircat(p, session.cwd, dir, NULL);

  /* Check for any expandable variables. */
  if (dir)
    dir = path_subst_uservar(p, &dir);

   return dir;
 }
