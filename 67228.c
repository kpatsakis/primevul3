static void _free_resources(r_pe_resource *rs) {
	if (rs) {
		free (rs->timestr);
		free (rs->data);
		free (rs->type);
		free (rs->language);
		free (rs);
	}
}
