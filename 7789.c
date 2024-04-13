construct_filtered_key(const char *repos_name,
                       const char *user,
                       const svn_membuf_t *authz_id,
                       apr_pool_t *result_pool)
{
  svn_membuf_t *result = apr_pcalloc(result_pool, sizeof(*result));
  size_t repos_len = strlen(repos_name);
  size_t user_len = user ? strlen(user) : 1;
  const char *nullable_user = user ? user : "\0";
  size_t size = authz_id->size + repos_len + 1 + user_len + 1;

  svn_membuf__create(result, size, result_pool);
  result->size = size;

  memcpy(result->data, repos_name, repos_len + 1);
  size = repos_len + 1;
  memcpy((char *)result->data + size, nullable_user, user_len + 1);
  size += user_len + 1;
  memcpy((char *)result->data + size, authz_id->data, authz_id->size);

  return result;
}