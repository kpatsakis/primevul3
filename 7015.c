DLLIMPORT int cfg_opt_setcomment(cfg_opt_t *opt, char *comment)
{
	char *oldcomment, *newcomment;

	if (!opt || !comment) {
		errno = EINVAL;
		return CFG_FAIL;
	}

	oldcomment = opt->comment;
	newcomment = strdup(comment);
	if (!newcomment)
		return CFG_FAIL;

	if (oldcomment)
		free(oldcomment);
	opt->comment = newcomment;
	opt->flags |= CFGF_COMMENTS;
	opt->flags |= CFGF_MODIFIED;

	return CFG_SUCCESS;
}