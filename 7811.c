synchronized_authz_initialize(void *baton, apr_pool_t *pool)
{
#if APR_HAS_THREADS
  svn_boolean_t multi_threaded = TRUE;
#else
  svn_boolean_t multi_threaded = FALSE;
#endif

  SVN_ERR(svn_object_pool__create(&authz_pool, multi_threaded, pool));
  SVN_ERR(svn_object_pool__create(&filtered_pool, multi_threaded, pool));

  apr_pool_cleanup_register(pool, NULL, deinit_authz, apr_pool_cleanup_null);
  return SVN_NO_ERROR;
}