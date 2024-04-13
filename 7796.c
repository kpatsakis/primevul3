svn_repos_authz_parse2(svn_authz_t **authz_p,
                       svn_stream_t *stream,
                       svn_stream_t *groups_stream,
                       svn_repos_authz_warning_func_t warning_func,
                       void *warning_baton,
                       apr_pool_t *result_pool,
                       apr_pool_t *scratch_pool)
{
  svn_authz_t *authz = apr_pcalloc(result_pool, sizeof(*authz));
  authz->pool = result_pool;

  /* Parse the configuration and construct the full authz model from it. */
  SVN_ERR(svn_authz__parse(&authz->full, stream, groups_stream,
                           warning_func, warning_baton,
                           result_pool, scratch_pool));

  *authz_p = authz;
  return SVN_NO_ERROR;
}