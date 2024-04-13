static int cdrom_sysctl_info(struct ctl_table *ctl, int write,
                           void __user *buffer, size_t *lenp, loff_t *ppos)
{
	int pos;
	char *info = cdrom_sysctl_settings.info;
	const int max_size = sizeof(cdrom_sysctl_settings.info);
	
	if (!*lenp || (*ppos && !write)) {
		*lenp = 0;
		return 0;
	}

	mutex_lock(&cdrom_mutex);

	pos = sprintf(info, "CD-ROM information, " VERSION "\n");
	
	if (cdrom_print_info("\ndrive name:\t", 0, info, &pos, CTL_NAME))
		goto done;
	if (cdrom_print_info("\ndrive speed:\t", 0, info, &pos, CTL_SPEED))
		goto done;
	if (cdrom_print_info("\ndrive # of slots:", 0, info, &pos, CTL_SLOTS))
		goto done;
	if (cdrom_print_info("\nCan close tray:\t",
				CDC_CLOSE_TRAY, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan open tray:\t",
				CDC_OPEN_TRAY, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan lock tray:\t",
				CDC_LOCK, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan change speed:",
				CDC_SELECT_SPEED, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan select disk:",
				CDC_SELECT_DISC, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan read multisession:",
				CDC_MULTI_SESSION, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan read MCN:\t",
				CDC_MCN, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nReports media changed:",
				CDC_MEDIA_CHANGED, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan play audio:\t",
				CDC_PLAY_AUDIO, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan write CD-R:\t",
				CDC_CD_R, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan write CD-RW:",
				CDC_CD_RW, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan read DVD:\t",
				CDC_DVD, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan write DVD-R:",
				CDC_DVD_R, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan write DVD-RAM:",
				CDC_DVD_RAM, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan read MRW:\t",
				CDC_MRW, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan write MRW:\t",
				CDC_MRW_W, info, &pos, CTL_CAPABILITY))
		goto done;
	if (cdrom_print_info("\nCan write RAM:\t",
				CDC_RAM, info, &pos, CTL_CAPABILITY))
		goto done;
	if (!scnprintf(info + pos, max_size - pos, "\n\n"))
		goto done;
doit:
	mutex_unlock(&cdrom_mutex);
	return proc_dostring(ctl, write, buffer, lenp, ppos);
done:
	pr_info("info buffer too small\n");
	goto doit;
}
