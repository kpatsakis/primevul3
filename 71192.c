int ipmi_si_remove_by_dev(struct device *dev)
{
	struct smi_info *e;
	int rv = -ENOENT;

	mutex_lock(&smi_infos_lock);
	list_for_each_entry(e, &smi_infos, link) {
		if (e->io.dev == dev) {
			cleanup_one_si(e);
			rv = 0;
			break;
		}
	}
	mutex_unlock(&smi_infos_lock);

	return rv;
}
