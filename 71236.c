static unsigned char port_inb(const struct si_sm_io *io, unsigned int offset)
{
	unsigned int addr = io->addr_data;

	return inb(addr + (offset * io->regspacing));
}
