static void port_outl(const struct si_sm_io *io, unsigned int offset,
		      unsigned char b)
{
	unsigned int addr = io->addr_data;

	outl(b << io->regshift, addr+(offset * io->regspacing));
}
