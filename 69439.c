add_supp_group(const char *name, gid_t **groups, size_t *ngroups)
{
  struct group *gr;

  if (*ngroups >= NGROUPS_MAX)
    errx(EXIT_FAILURE,
	P_("specifying more than %d supplemental group is not possible",
	   "specifying more than %d supplemental groups is not possible",
	     NGROUPS_MAX - 1), NGROUPS_MAX - 1);

  gr = getgrnam(name);
  if (!gr)
    errx(EXIT_FAILURE, _("group %s does not exist"), name);

  *groups = xrealloc(*groups, sizeof(gid_t) * (*ngroups + 1));
  (*groups)[*ngroups] = gr->gr_gid;
  (*ngroups)++;

  return gr->gr_gid;
}
