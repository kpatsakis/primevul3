matches_filtered_tree(const authz_user_rules_t *authz,
                      const char *repos_name,
                      const char *user)
{
  /* Does the user match? */
  if (user)
    {
      if (authz->user == NULL || strcmp(user, authz->user))
        return FALSE;
    }
  else if (authz->user != NULL)
    return FALSE;

  /* Does the repository match as well? */
  return strcmp(repos_name, authz->repository) == 0;
}