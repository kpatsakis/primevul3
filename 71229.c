static unsigned char intf_mem_inw(const struct si_sm_io *io,
				  unsigned int offset)
{
	return (readw((io->addr)+(offset * io->regspacing)) >> io->regshift)
		& 0xff;
}
