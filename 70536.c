static void handle_message(int sd)
{
	struct ifsock *ifs;

	LIST_FOREACH(ifs, &il, link) {
		if (ifs->in != sd)
			continue;

		if (ifs->cb)
			ifs->cb(sd);
	}
}
