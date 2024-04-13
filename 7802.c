svn_repos_authz_read4(svn_authz_t **authz_p,
                      const char *path,
                      const char *groups_path,
                      svn_boolean_t must_exist,
                      svn_repos_t *repos_hint,
                      svn_repos_authz_warning_func_t warning_func,
                      void *warning_baton,
                      apr_pool_t *result_pool,
                      apr_pool_t *scratch_pool)
{
  svn_authz_t *authz = apr_pcalloc(result_pool, sizeof(*authz));
  authz->pool = result_pool;

  SVN_ERR(authz_read(&authz->full, &authz->authz_id, path, groups_path,
                     must_exist, repos_hint, warning_func, warning_baton,
                     result_pool, scratch_pool));

  *authz_p = authz;
  return SVN_NO_ERROR;
}