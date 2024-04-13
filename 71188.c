int ipmi_si_add_smi(struct si_sm_io *io)
{
	int rv = 0;
	struct smi_info *new_smi, *dup;

	if (!io->io_setup) {
		if (io->addr_type == IPMI_IO_ADDR_SPACE) {
			io->io_setup = ipmi_si_port_setup;
		} else if (io->addr_type == IPMI_MEM_ADDR_SPACE) {
			io->io_setup = ipmi_si_mem_setup;
		} else {
			return -EINVAL;
		}
	}

	new_smi = kzalloc(sizeof(*new_smi), GFP_KERNEL);
	if (!new_smi)
		return -ENOMEM;
	spin_lock_init(&new_smi->si_lock);

	new_smi->io = *io;

	mutex_lock(&smi_infos_lock);
	dup = find_dup_si(new_smi);
	if (dup) {
		if (new_smi->io.addr_source == SI_ACPI &&
		    dup->io.addr_source == SI_SMBIOS) {
			/* We prefer ACPI over SMBIOS. */
			dev_info(dup->io.dev,
				 "Removing SMBIOS-specified %s state machine in favor of ACPI\n",
				 si_to_str[new_smi->io.si_type]);
			cleanup_one_si(dup);
		} else {
			dev_info(new_smi->io.dev,
				 "%s-specified %s state machine: duplicate\n",
				 ipmi_addr_src_to_str(new_smi->io.addr_source),
				 si_to_str[new_smi->io.si_type]);
			rv = -EBUSY;
			kfree(new_smi);
			goto out_err;
		}
	}

	pr_info("Adding %s-specified %s state machine\n",
		ipmi_addr_src_to_str(new_smi->io.addr_source),
		si_to_str[new_smi->io.si_type]);

	list_add_tail(&new_smi->link, &smi_infos);

	if (initialized)
		rv = try_smi_init(new_smi);
out_err:
	mutex_unlock(&smi_infos_lock);
	return rv;
}
