construct_authz_key(const svn_checksum_t *authz_key,
                    const svn_checksum_t *groups_key,
                    apr_pool_t *result_pool)
{
  svn_membuf_t *result = apr_pcalloc(result_pool, sizeof(*result));
  if (groups_key)
    {
      apr_size_t authz_size = svn_checksum_size(authz_key);
      apr_size_t groups_size = svn_checksum_size(groups_key);

      svn_membuf__create(result, authz_size + groups_size, result_pool);
      result->size = authz_size + groups_size; /* exact length is required! */

      memcpy(result->data, authz_key->digest, authz_size);
      memcpy((char *)result->data + authz_size,
             groups_key->digest, groups_size);
    }
  else
    {
      apr_size_t size = svn_checksum_size(authz_key);
      svn_membuf__create(result, size, result_pool);
      result->size = size; /* exact length is required! */
      memcpy(result->data, authz_key->digest, size);
    }

  return result;
}