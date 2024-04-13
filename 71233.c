static void mem_cleanup(struct si_sm_io *io)
{
	if (io->addr) {
		iounmap(io->addr);
		mem_region_cleanup(io, io->io_size);
	}
}
