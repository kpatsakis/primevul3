static void cdrom_sysctl_unregister(void)
{
	if (cdrom_sysctl_header)
		unregister_sysctl_table(cdrom_sysctl_header);
}
