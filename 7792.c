authz_read(authz_full_t **authz_p,
           svn_membuf_t **authz_id,
           const char *path,
           const char *groups_path,
           svn_boolean_t must_exist,
           svn_repos_t *repos_hint,
           svn_repos_authz_warning_func_t warning_func,
           void *warning_baton,
           apr_pool_t *result_pool,
           apr_pool_t *scratch_pool)
{
  svn_error_t* err = NULL;
  svn_stream_t *rules_stream = NULL;
  svn_stream_t *groups_stream = NULL;
  svn_checksum_t *rules_checksum = NULL;
  svn_checksum_t *groups_checksum = NULL;

  config_access_t *config_access =
    svn_repos__create_config_access(repos_hint, scratch_pool);

  /* Open the main authz file */
  SVN_ERR(svn_repos__get_config(&rules_stream, &rules_checksum, config_access,
                                path, must_exist, scratch_pool));

  /* Open the optional groups file */
  if (groups_path)
    SVN_ERR(svn_repos__get_config(&groups_stream, &groups_checksum,
                                  config_access, groups_path, must_exist,
                                  scratch_pool));

  /* The authz cache is optional. */
  *authz_id = construct_authz_key(rules_checksum, groups_checksum,
                                  result_pool);
  if (authz_pool)
    {
      /* Cache lookup. */
      SVN_ERR(svn_object_pool__lookup((void **)authz_p, authz_pool,
                                      *authz_id, result_pool));

      /* If not found, parse and add to cache. */
      if (!*authz_p)
        {
          apr_pool_t *item_pool = svn_object_pool__new_item_pool(authz_pool);

          /* Parse the configuration(s) and construct the full authz model
           * from it. */
          err = svn_authz__parse(authz_p, rules_stream, groups_stream,
                                 warning_func, warning_baton,
                                 item_pool, scratch_pool);
          if (err != SVN_NO_ERROR)
            {
              /* That pool would otherwise never get destroyed. */
              svn_pool_destroy(item_pool);

              /* Add the URL / file name to the error stack since the parser
               * doesn't have it. */
              err = svn_error_quick_wrapf(err,
                                   "Error while parsing config file: '%s':",
                                   path);
            }
          else
            {
              SVN_ERR(svn_object_pool__insert((void **)authz_p, authz_pool,
                                              *authz_id, *authz_p,
                                              item_pool, result_pool));
            }
        }
    }
  else
    {
      /* Parse the configuration(s) and construct the full authz model from
       * it. */
      err = svn_error_quick_wrapf(
          svn_authz__parse(authz_p, rules_stream, groups_stream,
                           warning_func, warning_baton,
                           result_pool, scratch_pool),
          "Error while parsing authz file: '%s':", path);
    }

  svn_repos__destroy_config_access(config_access);

  return err;
}