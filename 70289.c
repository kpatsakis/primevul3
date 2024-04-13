get_opthdr_from_block_data(pcap_t *p, struct block_cursor *cursor, char *errbuf)
{
	struct option_header *opthdr;

	opthdr = get_from_block_data(cursor, sizeof(*opthdr), errbuf);
	if (opthdr == NULL) {
		/*
		 * Option header is cut short.
		 */
		return (NULL);
	}

	/*
	 * Byte-swap it if necessary.
	 */
	if (p->swapped) {
		opthdr->option_code = SWAPSHORT(opthdr->option_code);
		opthdr->option_length = SWAPSHORT(opthdr->option_length);
	}

	return (opthdr);
}
