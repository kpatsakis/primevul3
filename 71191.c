void ipmi_si_remove_by_data(int addr_space, enum si_type si_type,
			    unsigned long addr)
{
	/* remove */
	struct smi_info *e, *tmp_e;

	mutex_lock(&smi_infos_lock);
	list_for_each_entry_safe(e, tmp_e, &smi_infos, link) {
		if (e->io.addr_type != addr_space)
			continue;
		if (e->io.si_type != si_type)
			continue;
		if (e->io.addr_data == addr)
			cleanup_one_si(e);
	}
	mutex_unlock(&smi_infos_lock);
}
