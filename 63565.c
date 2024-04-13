static int cdrom_print_info(const char *header, int val, char *info,
				int *pos, enum cdrom_print_option option)
{
	const int max_size = sizeof(cdrom_sysctl_settings.info);
	struct cdrom_device_info *cdi;
	int ret;

	ret = scnprintf(info + *pos, max_size - *pos, header);
	if (!ret)
		return 1;

	*pos += ret;

	list_for_each_entry(cdi, &cdrom_list, list) {
		switch (option) {
		case CTL_NAME:
			ret = scnprintf(info + *pos, max_size - *pos,
					"\t%s", cdi->name);
			break;
		case CTL_SPEED:
			ret = scnprintf(info + *pos, max_size - *pos,
					"\t%d", cdi->speed);
			break;
		case CTL_SLOTS:
			ret = scnprintf(info + *pos, max_size - *pos,
					"\t%d", cdi->capacity);
			break;
		case CTL_CAPABILITY:
			ret = scnprintf(info + *pos, max_size - *pos,
					"\t%d", CDROM_CAN(val) != 0);
			break;
		default:
			pr_info("invalid option%d\n", option);
			return 1;
		}
		if (!ret)
			return 1;
		*pos += ret;
	}

	return 0;
}
