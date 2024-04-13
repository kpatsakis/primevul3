static void port_cleanup(struct si_sm_io *io)
{
	unsigned int addr = io->addr_data;
	int          idx;

	if (addr) {
		for (idx = 0; idx < io->io_size; idx++)
			release_region(addr + idx * io->regspacing,
				       io->regsize);
	}
}
