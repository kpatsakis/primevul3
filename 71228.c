static unsigned char intf_mem_inl(const struct si_sm_io *io,
				  unsigned int offset)
{
	return (readl((io->addr)+(offset * io->regspacing)) >> io->regshift)
		& 0xff;
}
