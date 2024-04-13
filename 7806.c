filter_tree(svn_authz_t *authz,
            apr_pool_t *scratch_pool)
{
  apr_pool_t *pool = authz->filtered->pool;
  const char *repos_name = authz->filtered->repository;
  const char *user = authz->filtered->user;
  node_t *root;

  if (filtered_pool)
    {
      svn_membuf_t *key = construct_filtered_key(repos_name, user,
                                                 authz->authz_id,
                                                 scratch_pool);

      /* Cache lookup. */
      SVN_ERR(svn_object_pool__lookup((void **)&root, filtered_pool, key,
                                      pool));

      if (!root)
        {
          apr_pool_t *item_pool = svn_object_pool__new_item_pool(authz_pool);
          authz_full_t *add_ref = NULL;

          /* Make sure the underlying full authz object lives as long as the
           * filtered one that we are about to create.  We do this by adding
           * a reference to it in ITEM_POOL (which may live longer than AUTHZ).
           *
           * Note that we already have a reference to that full authz in
           * AUTHZ->FULL. Assert that we actually don't created multiple
           * instances of the same full model.
           */
          svn_error_clear(svn_object_pool__lookup((void **)&add_ref,
                                                  authz_pool, authz->authz_id,
                                                  item_pool));
          SVN_ERR_ASSERT(add_ref == authz->full);

          /* Now construct the new filtered tree and cache it. */
          root = create_user_authz(authz->full, repos_name, user, item_pool,
                                   scratch_pool);
          svn_error_clear(svn_object_pool__insert((void **)&root,
                                                  filtered_pool, key, root,
                                                  item_pool, pool));
        }
     }
  else
    {
      root = create_user_authz(authz->full, repos_name, user, pool,
                               scratch_pool);
    }

  /* Write a new entry. */
  authz->filtered->root = root;

  return SVN_NO_ERROR;
}