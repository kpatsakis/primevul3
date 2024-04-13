static int init_ipmi_si(void)
{
	struct smi_info *e;
	enum ipmi_addr_src type = SI_INVALID;

	if (initialized)
		return 0;

	pr_info("IPMI System Interface driver\n");

	/* If the user gave us a device, they presumably want us to use it */
	if (!ipmi_si_hardcode_find_bmc())
		goto do_scan;

	ipmi_si_platform_init();

	ipmi_si_pci_init();

	ipmi_si_parisc_init();

	/* We prefer devices with interrupts, but in the case of a machine
	   with multiple BMCs we assume that there will be several instances
	   of a given type so if we succeed in registering a type then also
	   try to register everything else of the same type */
do_scan:
	mutex_lock(&smi_infos_lock);
	list_for_each_entry(e, &smi_infos, link) {
		/* Try to register a device if it has an IRQ and we either
		   haven't successfully registered a device yet or this
		   device has the same type as one we successfully registered */
		if (e->io.irq && (!type || e->io.addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->io.addr_source;
			}
		}
	}

	/* type will only have been set if we successfully registered an si */
	if (type)
		goto skip_fallback_noirq;

	/* Fall back to the preferred device */

	list_for_each_entry(e, &smi_infos, link) {
		if (!e->io.irq && (!type || e->io.addr_source == type)) {
			if (!try_smi_init(e)) {
				type = e->io.addr_source;
			}
		}
	}

skip_fallback_noirq:
	initialized = 1;
	mutex_unlock(&smi_infos_lock);

	if (type)
		return 0;

	mutex_lock(&smi_infos_lock);
	if (unload_when_empty && list_empty(&smi_infos)) {
		mutex_unlock(&smi_infos_lock);
		cleanup_ipmi_si();
		pr_warn("Unable to find any System Interface(s)\n");
		return -ENODEV;
	} else {
		mutex_unlock(&smi_infos_lock);
		return 0;
	}
}
