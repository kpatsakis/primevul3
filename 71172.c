static void check_set_rcv_irq(struct smi_info *smi_info)
{
	u8 enables = 0;
	int rv;

	if (!smi_info->io.irq)
		return;

	rv = get_global_enables(smi_info, &enables);
	if (!rv) {
		enables |= IPMI_BMC_RCV_MSG_INTR;
		rv = set_global_enables(smi_info, enables);
	}

	if (rv < 0) {
		dev_err(smi_info->io.dev,
			"Cannot check setting the rcv irq: %d\n", rv);
		return;
	}

	if (rv) {
		/*
		 * An error when setting the event buffer bit means
		 * setting the bit is not supported.
		 */
		dev_warn(smi_info->io.dev,
			 "The BMC does not support setting the recv irq bit, compensating, but the BMC needs to be fixed.\n");
		smi_info->cannot_disable_irq = true;
		smi_info->irq_enable_broken = true;
	}
}
