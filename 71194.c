int ipmi_std_irq_setup(struct si_sm_io *io)
{
	int rv;

	if (!io->irq)
		return 0;

	rv = request_irq(io->irq,
			 ipmi_si_irq_handler,
			 IRQF_SHARED,
			 DEVICE_NAME,
			 io->irq_handler_data);
	if (rv) {
		dev_warn(io->dev, "%s unable to claim interrupt %d,"
			 " running polled\n",
			 DEVICE_NAME, io->irq);
		io->irq = 0;
	} else {
		io->irq_cleanup = std_irq_cleanup;
		ipmi_irq_finish_setup(io);
		dev_info(io->dev, "Using irq %d\n", io->irq);
	}

	return rv;
}
