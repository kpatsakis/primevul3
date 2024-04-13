static void std_irq_cleanup(struct si_sm_io *io)
{
	ipmi_irq_start_cleanup(io);
	free_irq(io->irq, io->irq_handler_data);
}
