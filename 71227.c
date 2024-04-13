static unsigned char intf_mem_inb(const struct si_sm_io *io,
				  unsigned int offset)
{
	return readb((io->addr)+(offset * io->regspacing));
}
