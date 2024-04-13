static void mark(void)
{
	struct ifsock *ifs;

	LIST_FOREACH(ifs, &il, link) {
		if (ifs->out != -1)
			ifs->stale = 1;
		else
			ifs->stale = 0;
	}
}
