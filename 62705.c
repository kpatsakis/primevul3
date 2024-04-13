struct bug_entry *find_bug(unsigned long bugaddr)
{
	struct bug_entry *bug;

	for (bug = __start___bug_table; bug < __stop___bug_table; ++bug)
		if (bugaddr == bug->bug_addr)
			return bug;
	return module_find_bug(bugaddr);
}
