deinit_authz(void *data)
{
  /* The two object pools run their own cleanup handlers. */
  authz_pool = NULL;
  filtered_pool = NULL;
  authz_pool_initialized = FALSE;
  return APR_SUCCESS;
}