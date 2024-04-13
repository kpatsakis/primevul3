static unsigned char mem_inq(const struct si_sm_io *io, unsigned int offset)
{
	return (readq((io->addr)+(offset * io->regspacing)) >> io->regshift)
		& 0xff;
}
