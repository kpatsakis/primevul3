static void intf_mem_outb(const struct si_sm_io *io, unsigned int offset,
			  unsigned char b)
{
	writeb(b, (io->addr)+(offset * io->regspacing));
}
