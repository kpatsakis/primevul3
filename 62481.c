static int setup_e820_entries(struct boot_params *params)
{
	unsigned int nr_e820_entries;

	nr_e820_entries = e820_saved.nr_map;

	/* TODO: Pass entries more than E820MAX in bootparams setup data */
	if (nr_e820_entries > E820MAX)
		nr_e820_entries = E820MAX;

	params->e820_entries = nr_e820_entries;
	memcpy(&params->e820_map, &e820_saved.map,
	       nr_e820_entries * sizeof(struct e820entry));

	return 0;
}
