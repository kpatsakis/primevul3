static void port_outw(const struct si_sm_io *io, unsigned int offset,
		      unsigned char b)
{
	unsigned int addr = io->addr_data;

	outw(b << io->regshift, addr + (offset * io->regspacing));
}
