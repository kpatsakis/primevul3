process_idb_options(pcap_t *p, struct block_cursor *cursor, uint64_t *tsresol,
    uint64_t *tsoffset, int *is_binary, char *errbuf)
{
	struct option_header *opthdr;
	void *optvalue;
	int saw_tsresol, saw_tsoffset;
	uint8_t tsresol_opt;
	u_int i;

	saw_tsresol = 0;
	saw_tsoffset = 0;
	while (cursor->data_remaining != 0) {
		/*
		 * Get the option header.
		 */
		opthdr = get_opthdr_from_block_data(p, cursor, errbuf);
		if (opthdr == NULL) {
			/*
			 * Option header is cut short.
			 */
			return (-1);
		}

		/*
		 * Get option value.
		 */
		optvalue = get_optvalue_from_block_data(cursor, opthdr,
		    errbuf);
		if (optvalue == NULL) {
			/*
			 * Option value is cut short.
			 */
			return (-1);
		}

		switch (opthdr->option_code) {

		case OPT_ENDOFOPT:
			if (opthdr->option_length != 0) {
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "Interface Description Block has opt_endofopt option with length %u != 0",
				    opthdr->option_length);
				return (-1);
			}
			goto done;

		case IF_TSRESOL:
			if (opthdr->option_length != 1) {
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "Interface Description Block has if_tsresol option with length %u != 1",
				    opthdr->option_length);
				return (-1);
			}
			if (saw_tsresol) {
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "Interface Description Block has more than one if_tsresol option");
				return (-1);
			}
			saw_tsresol = 1;
			memcpy(&tsresol_opt, optvalue, sizeof(tsresol_opt));
			if (tsresol_opt & 0x80) {
				/*
				 * Resolution is negative power of 2.
				 */
				uint8_t tsresol_shift = (tsresol_opt & 0x7F);

				if (tsresol_shift > 63) {
					/*
					 * Resolution is too high; 2^-{res}
					 * won't fit in a 64-bit value.
					 */
					pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
					    "Interface Description Block if_tsresol option resolution 2^-%u is too high",
					    tsresol_shift);
					return (-1);
				}
				*is_binary = 1;
				*tsresol = ((uint64_t)1) << tsresol_shift;
			} else {
				/*
				 * Resolution is negative power of 10.
				 */
				if (tsresol_opt > 19) {
					/*
					 * Resolution is too high; 2^-{res}
					 * won't fit in a 64-bit value (the
					 * largest power of 10 that fits
					 * in a 64-bit value is 10^19, as
					 * the largest 64-bit unsigned
					 * value is ~1.8*10^19).
					 */
					pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
					    "Interface Description Block if_tsresol option resolution 10^-%u is too high",
					    tsresol_opt);
					return (-1);
				}
				*is_binary = 0;
				*tsresol = 1;
				for (i = 0; i < tsresol_opt; i++)
					*tsresol *= 10;
			}
			break;

		case IF_TSOFFSET:
			if (opthdr->option_length != 8) {
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "Interface Description Block has if_tsoffset option with length %u != 8",
				    opthdr->option_length);
				return (-1);
			}
			if (saw_tsoffset) {
				pcap_snprintf(errbuf, PCAP_ERRBUF_SIZE,
				    "Interface Description Block has more than one if_tsoffset option");
				return (-1);
			}
			saw_tsoffset = 1;
			memcpy(tsoffset, optvalue, sizeof(*tsoffset));
			if (p->swapped)
				*tsoffset = SWAPLL(*tsoffset);
			break;

		default:
			break;
		}
	}

done:
	return (0);
}
