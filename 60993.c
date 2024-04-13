static int append_symref(const char **out, git_vector *symrefs, const char *ptr)
{
	int error;
	const char *end;
	git_buf buf = GIT_BUF_INIT;
	git_refspec *mapping = NULL;

	ptr += strlen(GIT_CAP_SYMREF);
	if (*ptr != '=')
		goto on_invalid;

	ptr++;
	if (!(end = strchr(ptr, ' ')) &&
	    !(end = strchr(ptr, '\0')))
		goto on_invalid;

	if ((error = git_buf_put(&buf, ptr, end - ptr)) < 0)
		return error;

	/* symref mapping has refspec format */
	mapping = git__calloc(1, sizeof(git_refspec));
	GITERR_CHECK_ALLOC(mapping);

	error = git_refspec__parse(mapping, git_buf_cstr(&buf), true);
	git_buf_free(&buf);

	/* if the error isn't OOM, then it's a parse error; let's use a nicer message */
	if (error < 0) {
		if (giterr_last()->klass != GITERR_NOMEMORY)
			goto on_invalid;

		git__free(mapping);
		return error;
	}

	if ((error = git_vector_insert(symrefs, mapping)) < 0)
		return error;

	*out = end;
	return 0;

on_invalid:
	giterr_set(GITERR_NET, "remote sent invalid symref");
	git_refspec__free(mapping);
	git__free(mapping);
	return -1;
}
