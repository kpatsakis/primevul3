get_optvalue_from_block_data(struct block_cursor *cursor,
    struct option_header *opthdr, char *errbuf)
{
	size_t padded_option_len;
	void *optvalue;

	/* Pad option length to 4-byte boundary */
	padded_option_len = opthdr->option_length;
	padded_option_len = ((padded_option_len + 3)/4)*4;

	optvalue = get_from_block_data(cursor, padded_option_len, errbuf);
	if (optvalue == NULL) {
		/*
		 * Option value is cut short.
		 */
		return (NULL);
	}

	return (optvalue);
}
