svn_repos_authz_check_access(svn_authz_t *authz, const char *repos_name,
                             const char *path, const char *user,
                             svn_repos_authz_access_t required_access,
                             svn_boolean_t *access_granted,
                             apr_pool_t *pool)
{
  const authz_access_t required =
    ((required_access & svn_authz_read ? authz_access_read_flag : 0)
     | (required_access & svn_authz_write ? authz_access_write_flag : 0));

  /* Pick or create the suitable pre-filtered path rule tree. */
  authz_user_rules_t *rules = get_user_rules(
      authz,
      (repos_name ? repos_name : AUTHZ_ANY_REPOSITORY),
      user);

  /* In many scenarios, users have uniform access to a repository
   * (blanket access or no access at all).
   *
   * In these cases, don't bother creating or consulting the filtered tree.
   */
  if ((rules->global_rights.min_access & required) == required)
    {
      *access_granted = TRUE;
      return SVN_NO_ERROR;
    }

  if ((rules->global_rights.max_access & required) != required)
    {
      *access_granted = FALSE;
      return SVN_NO_ERROR;
    }

  /* No specific path given, i.e. looking for anywhere in the tree? */
  if (!path)
    {
      *access_granted =
        ((rules->global_rights.max_access & required) == required);
      return SVN_NO_ERROR;
    }

  /* Rules tree lookup */

  /* Did we already filter the data model? */
  if (!rules->root)
    SVN_ERR(filter_tree(authz, pool));

  /* Re-use previous lookup results, if possible. */
  path = init_lockup_state(authz->filtered->lookup_state,
                           authz->filtered->root, path);

  /* Sanity check. */
  SVN_ERR_ASSERT(path[0] == '/');

  /* Determine the granted access for the requested path.
   * PATH does not need to be normalized for lockup(). */
  *access_granted = lookup(rules->lookup_state, path, required,
                           !!(required_access & svn_authz_recursive), pool);

  return SVN_NO_ERROR;
}