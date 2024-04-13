static void intf_mem_outl(const struct si_sm_io *io, unsigned int offset,
			  unsigned char b)
{
	writel(b << io->regshift, (io->addr)+(offset * io->regspacing));
}
