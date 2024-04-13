create_user_authz(authz_full_t *authz,
                  const char *repository,
                  const char *user,
                  apr_pool_t *result_pool,
                  apr_pool_t *scratch_pool)
{
  int i;
  node_t *root = create_node(NULL, result_pool);
  construction_context_t *ctx = create_construction_context(scratch_pool);

  /* Use a separate sub-pool to keep memory usage tight. */
  apr_pool_t *subpool = svn_pool_create(scratch_pool);

  /* Find all ACLs for REPOSITORY. */
  apr_array_header_t *acls = apr_array_make(subpool, authz->acls->nelts,
                                            sizeof(authz_acl_t *));
  for (i = 0; i < authz->acls->nelts; ++i)
    {
      const authz_acl_t *acl = &APR_ARRAY_IDX(authz->acls, i, authz_acl_t);
      if (svn_authz__acl_applies_to_repo(acl, repository))
        {
          /* ACLs in the AUTHZ are sorted by path and repository.
           * So, if there is a rule for the repo and a global rule for the
           * same path, we will detect them here. */
          if (acls->nelts)
            {
              const authz_acl_t *prev_acl
                = APR_ARRAY_IDX(acls, acls->nelts - 1, const authz_acl_t *);
              if (svn_authz__compare_paths(&prev_acl->rule, &acl->rule) == 0)
                {
                  svn_boolean_t global_acl_applies;
                  svn_boolean_t repos_acl_applies;

                  /* Previous ACL is a global rule. */
                  SVN_ERR_ASSERT_NO_RETURN(!strcmp(prev_acl->rule.repos,
                                                   AUTHZ_ANY_REPOSITORY));
                  /* Current ACL is a per-repository rule. */
                  SVN_ERR_ASSERT_NO_RETURN(strcmp(acl->rule.repos,
                                                  AUTHZ_ANY_REPOSITORY));

                  global_acl_applies =
                    svn_authz__get_acl_access(NULL, prev_acl, user, repository);
                  repos_acl_applies =
                    svn_authz__get_acl_access(NULL, acl, user, repository);

                  /* Prefer rules which apply to both this user and this path
                   * over rules which apply only to the path. In cases where
                   * both rules apply to user and path, always prefer the
                   * repository-specific rule. */
                  if (!global_acl_applies || repos_acl_applies)
                    {
                      apr_array_pop(acls);
                      APR_ARRAY_PUSH(acls, const authz_acl_t *) = acl;
                    }
                }
              else
                APR_ARRAY_PUSH(acls, const authz_acl_t *) = acl;
            }
          else
            APR_ARRAY_PUSH(acls, const authz_acl_t *) = acl;
        }
    }

  /* Filtering and tree construction. */
  for (i = 0; i < acls->nelts; ++i)
    process_acl(ctx, APR_ARRAY_IDX(acls, i, const authz_acl_t *),
                root, repository, user, result_pool, subpool);

  /* If there is no relevant rule at the root node, the "no access" default
   * applies. Give it a SEQUENCE_NUMBER that will never overrule others. */
  if (!has_local_rule(&root->rights))
    {
      root->rights.access.sequence_number = 0;
      root->rights.access.rights = authz_access_none;
    }

  /* Trim the tree.
   *
   * We can't do pattern comparison, so for most pattern rules we cannot
   * say that a set of rules "eclipses" / overrides a given other set of
   * rules for all possible paths.  That limits the accuracy of our check
   * for recursive access in similar ways than for non-pattern rules.
   *
   * However, the user expects a rule ending with "**" to eclipse any older
   * rule in that sub-tree recursively.  So, this trim function removes
   * eclipsed nodes from the tree.
   */
  svn_pool_clear(subpool);
  trim_tree(root, NO_SEQUENCE_NUMBER, subpool);

  /* Calculate recursive rights.
   *
   * This is a bottom-up calculation of the range of access rights
   * specified anywhere in  the respective sub-tree, including the base
   * node itself.
   *
   * To prevent additional finalization passes, we piggy-back the addition
   * of the ordering links of the prefix and suffix sub-node rules.
   */
  svn_pool_clear(subpool);
  finalize_tree(root, &root->rights, subpool);

  /* Done. */
  svn_pool_destroy(subpool);
  return root;
}