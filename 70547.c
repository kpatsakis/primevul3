static int sweep(void)
{
	int modified = 0;
	struct ifsock *ifs, *tmp;

	LIST_FOREACH_SAFE(ifs, &il, link, tmp) {
		if (!ifs->stale)
			continue;

		modified++;
		logit(LOG_DEBUG, "Removing stale ifs %s", inet_ntoa(ifs->addr.sin_addr));

		LIST_REMOVE(ifs, link);
		close(ifs->out);
		free(ifs);
	}

	return modified;
}
