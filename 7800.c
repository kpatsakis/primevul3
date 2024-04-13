svn_repos_authz_initialize(apr_pool_t *pool)
{
  /* Protect against multiple calls. */
  return svn_error_trace(svn_atomic__init_once(&authz_pool_initialized,
                                               synchronized_authz_initialize,
                                               NULL, pool));
}