static int close_socket(void)
{
	int ret = 0;
	struct ifsock *ifs, *tmp;

	LIST_FOREACH_SAFE(ifs, &il, link, tmp) {
		LIST_REMOVE(ifs, link);
		if (ifs->out != -1)
			ret |= close(ifs->out);
		else
			ret |= close(ifs->in);
		free(ifs);
	}

	return ret;
}
