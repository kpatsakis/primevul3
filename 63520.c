static void __exit cdrom_exit(void)
{
	pr_info("Uniform CD-ROM driver unloaded\n");
	cdrom_sysctl_unregister();
}
