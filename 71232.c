static void intf_mem_outw(const struct si_sm_io *io, unsigned int offset,
			  unsigned char b)
{
	writeb(b << io->regshift, (io->addr)+(offset * io->regspacing));
}
