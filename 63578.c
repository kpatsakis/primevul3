static int cdrom_sysctl_handler(struct ctl_table *ctl, int write,
				void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int ret;
	
	ret = proc_dointvec(ctl, write, buffer, lenp, ppos);

	if (write) {
	
		/* we only care for 1 or 0. */
		autoclose        = !!cdrom_sysctl_settings.autoclose;
		autoeject        = !!cdrom_sysctl_settings.autoeject;
		debug	         = !!cdrom_sysctl_settings.debug;
		lockdoor         = !!cdrom_sysctl_settings.lock;
		check_media_type = !!cdrom_sysctl_settings.check;

		/* update the option flags according to the changes. we
		   don't have per device options through sysctl yet,
		   but we will have and then this will disappear. */
		cdrom_update_settings();
	}

        return ret;
}
