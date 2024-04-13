pcap_ng_next_packet(pcap_t *p, struct pcap_pkthdr *hdr, u_char **data)
{
	struct pcap_ng_sf *ps = p->priv;
	struct block_cursor cursor;
	int status;
	struct enhanced_packet_block *epbp;
	struct simple_packet_block *spbp;
	struct packet_block *pbp;
	bpf_u_int32 interface_id = 0xFFFFFFFF;
	struct interface_description_block *idbp;
	struct section_header_block *shbp;
	FILE *fp = p->rfile;
	uint64_t t, sec, frac;

	/*
	 * Look for an Enhanced Packet Block, a Simple Packet Block,
	 * or a Packet Block.
	 */
	for (;;) {
		/*
		 * Read the block type and length; those are common
		 * to all blocks.
		 */
		status = read_block(fp, p, &cursor, p->errbuf);
		if (status == 0)
			return (1);	/* EOF */
		if (status == -1)
			return (-1);	/* error */
		switch (cursor.block_type) {

		case BT_EPB:
			/*
			 * Get a pointer to the fixed-length portion of the
			 * EPB.
			 */
			epbp = get_from_block_data(&cursor, sizeof(*epbp),
			    p->errbuf);
			if (epbp == NULL)
				return (-1);	/* error */

			/*
			 * Byte-swap it if necessary.
			 */
			if (p->swapped) {
				/* these were written in opposite byte order */
				interface_id = SWAPLONG(epbp->interface_id);
				hdr->caplen = SWAPLONG(epbp->caplen);
				hdr->len = SWAPLONG(epbp->len);
				t = ((uint64_t)SWAPLONG(epbp->timestamp_high)) << 32 |
				    SWAPLONG(epbp->timestamp_low);
			} else {
				interface_id = epbp->interface_id;
				hdr->caplen = epbp->caplen;
				hdr->len = epbp->len;
				t = ((uint64_t)epbp->timestamp_high) << 32 |
				    epbp->timestamp_low;
			}
			goto found;

		case BT_SPB:
			/*
			 * Get a pointer to the fixed-length portion of the
			 * SPB.
			 */
			spbp = get_from_block_data(&cursor, sizeof(*spbp),
			    p->errbuf);
			if (spbp == NULL)
				return (-1);	/* error */

			/*
			 * SPB packets are assumed to have arrived on
			 * the first interface.
			 */
			interface_id = 0;

			/*
			 * Byte-swap it if necessary.
			 */
			if (p->swapped) {
				/* these were written in opposite byte order */
				hdr->len = SWAPLONG(spbp->len);
			} else
				hdr->len = spbp->len;

			/*
			 * The SPB doesn't give the captured length;
			 * it's the minimum of the snapshot length
			 * and the packet length.
			 */
			hdr->caplen = hdr->len;
			if (hdr->caplen > (bpf_u_int32)p->snapshot)
				hdr->caplen = p->snapshot;
			t = 0;	/* no time stamps */
			goto found;

		case BT_PB:
			/*
			 * Get a pointer to the fixed-length portion of the
			 * PB.
			 */
			pbp = get_from_block_data(&cursor, sizeof(*pbp),
			    p->errbuf);
			if (pbp == NULL)
				return (-1);	/* error */

			/*
			 * Byte-swap it if necessary.
			 */
			if (p->swapped) {
				/* these were written in opposite byte order */
				interface_id = SWAPSHORT(pbp->interface_id);
				hdr->caplen = SWAPLONG(pbp->caplen);
				hdr->len = SWAPLONG(pbp->len);
				t = ((uint64_t)SWAPLONG(pbp->timestamp_high)) << 32 |
				    SWAPLONG(pbp->timestamp_low);
			} else {
				interface_id = pbp->interface_id;
				hdr->caplen = pbp->caplen;
				hdr->len = pbp->len;
				t = ((uint64_t)pbp->timestamp_high) << 32 |
				    pbp->timestamp_low;
			}
			goto found;

		case BT_IDB:
			/*
			 * Interface Description Block.  Get a pointer
			 * to its fixed-length portion.
			 */
			idbp = get_from_block_data(&cursor, sizeof(*idbp),
			    p->errbuf);
			if (idbp == NULL)
				return (-1);	/* error */

			/*
			 * Byte-swap it if necessary.
			 */
			if (p->swapped) {
				idbp->linktype = SWAPSHORT(idbp->linktype);
				idbp->snaplen = SWAPLONG(idbp->snaplen);
			}

			/*
			 * If the link-layer type or snapshot length
			 * differ from the ones for the first IDB we
			 * saw, quit.
			 *
			 * XXX - just discard packets from those
			 * interfaces?
			 */
			if (p->linktype != idbp->linktype) {
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "an interface has a type %u different from the type of the first interface",
				    idbp->linktype);
				return (-1);
			}

			/*
			 * Check against the *adjusted* value of this IDB's
			 * snapshot length.
			 */
			if ((bpf_u_int32)p->snapshot !=
			    pcap_adjust_snapshot(p->linktype, idbp->snaplen)) {
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "an interface has a snapshot length %u different from the type of the first interface",
				    idbp->snaplen);
				return (-1);
			}

			/*
			 * Try to add this interface.
			 */
			if (!add_interface(p, &cursor, p->errbuf))
				return (-1);
			break;

		case BT_SHB:
			/*
			 * Section Header Block.  Get a pointer
			 * to its fixed-length portion.
			 */
			shbp = get_from_block_data(&cursor, sizeof(*shbp),
			    p->errbuf);
			if (shbp == NULL)
				return (-1);	/* error */

			/*
			 * Assume the byte order of this section is
			 * the same as that of the previous section.
			 * We'll check for that later.
			 */
			if (p->swapped) {
				shbp->byte_order_magic =
				    SWAPLONG(shbp->byte_order_magic);
				shbp->major_version =
				    SWAPSHORT(shbp->major_version);
			}

			/*
			 * Make sure the byte order doesn't change;
			 * pcap_is_swapped() shouldn't change its
			 * return value in the middle of reading a capture.
			 */
			switch (shbp->byte_order_magic) {

			case BYTE_ORDER_MAGIC:
				/*
				 * OK.
				 */
				break;

			case SWAPLONG(BYTE_ORDER_MAGIC):
				/*
				 * Byte order changes.
				 */
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "the file has sections with different byte orders");
				return (-1);

			default:
				/*
				 * Not a valid SHB.
				 */
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "the file has a section with a bad byte order magic field");
				return (-1);
			}

			/*
			 * Make sure the major version is the version
			 * we handle.
			 */
			if (shbp->major_version != PCAP_NG_VERSION_MAJOR) {
				pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
				    "unknown pcapng savefile major version number %u",
				    shbp->major_version);
				return (-1);
			}

			/*
			 * Reset the interface count; this section should
			 * have its own set of IDBs.  If any of them
			 * don't have the same interface type, snapshot
			 * length, or resolution as the first interface
			 * we saw, we'll fail.  (And if we don't see
			 * any IDBs, we'll fail when we see a packet
			 * block.)
			 */
			ps->ifcount = 0;
			break;

		default:
			/*
			 * Not a packet block, IDB, or SHB; ignore it.
			 */
			break;
		}
	}

found:
	/*
	 * Is the interface ID an interface we know?
	 */
	if (interface_id >= ps->ifcount) {
		/*
		 * Yes.  Fail.
		 */
		pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
		    "a packet arrived on interface %u, but there's no Interface Description Block for that interface",
		    interface_id);
		return (-1);
	}

	if (hdr->caplen > (bpf_u_int32)p->snapshot) {
		pcap_snprintf(p->errbuf, PCAP_ERRBUF_SIZE,
		    "invalid packet capture length %u, bigger than "
		    "snaplen of %d", hdr->caplen, p->snapshot);
		return (-1);
	}

	/*
	 * Convert the time stamp to seconds and fractions of a second,
	 * with the fractions being in units of the file-supplied resolution.
	 */
	sec = t / ps->ifaces[interface_id].tsresol + ps->ifaces[interface_id].tsoffset;
	frac = t % ps->ifaces[interface_id].tsresol;

	/*
	 * Convert the fractions from units of the file-supplied resolution
	 * to units of the user-requested resolution.
	 */
	switch (ps->ifaces[interface_id].scale_type) {

	case PASS_THROUGH:
		/*
		 * The interface resolution is what the user wants,
		 * so we're done.
		 */
		break;

	case SCALE_UP_DEC:
		/*
		 * The interface resolution is less than what the user
		 * wants; scale the fractional part up to the units of
		 * the resolution the user requested by multiplying by
		 * the quotient of the user-requested resolution and the
		 * file-supplied resolution.
		 *
		 * Those resolutions are both powers of 10, and the user-
		 * requested resolution is greater than the file-supplied
		 * resolution, so the quotient in question is an integer.
		 * We've calculated that quotient already, so we just
		 * multiply by it.
		 */
		frac *= ps->ifaces[interface_id].scale_factor;
		break;

	case SCALE_UP_BIN:
		/*
		 * The interface resolution is less than what the user
		 * wants; scale the fractional part up to the units of
		 * the resolution the user requested by multiplying by
		 * the quotient of the user-requested resolution and the
		 * file-supplied resolution.
		 *
		 * The file-supplied resolution is a power of 2, so the
		 * quotient is not an integer, so, in order to do this
		 * entirely with integer arithmetic, we multiply by the
		 * user-requested resolution and divide by the file-
		 * supplied resolution.
		 *
		 * XXX - Is there something clever we could do here,
		 * given that we know that the file-supplied resolution
		 * is a power of 2?  Doing a multiplication followed by
		 * a division runs the risk of overflowing, and involves
		 * two non-simple arithmetic operations.
		 */
		frac *= ps->user_tsresol;
		frac /= ps->ifaces[interface_id].tsresol;
		break;

	case SCALE_DOWN_DEC:
		/*
		 * The interface resolution is greater than what the user
		 * wants; scale the fractional part up to the units of
		 * the resolution the user requested by multiplying by
		 * the quotient of the user-requested resolution and the
		 * file-supplied resolution.
		 *
		 * Those resolutions are both powers of 10, and the user-
		 * requested resolution is less than the file-supplied
		 * resolution, so the quotient in question isn't an
		 * integer, but its reciprocal is, and we can just divide
		 * by the reciprocal of the quotient.  We've calculated
		 * the reciprocal of that quotient already, so we must
		 * divide by it.
		 */
		frac /= ps->ifaces[interface_id].scale_factor;
		break;


	case SCALE_DOWN_BIN:
		/*
		 * The interface resolution is greater than what the user
		 * wants; convert the fractional part to units of the
		 * resolution the user requested by multiplying by the
		 * quotient of the user-requested resolution and the
		 * file-supplied resolution.  We do that by multiplying
		 * by the user-requested resolution and dividing by the
		 * file-supplied resolution, as the quotient might not
		 * fit in an integer.
		 *
		 * The file-supplied resolution is a power of 2, so the
		 * quotient is not an integer, and neither is its
		 * reciprocal, so, in order to do this entirely with
		 * integer arithmetic, we multiply by the user-requested
		 * resolution and divide by the file-supplied resolution.
		 *
		 * XXX - Is there something clever we could do here,
		 * given that we know that the file-supplied resolution
		 * is a power of 2?  Doing a multiplication followed by
		 * a division runs the risk of overflowing, and involves
		 * two non-simple arithmetic operations.
		 */
		frac *= ps->user_tsresol;
		frac /= ps->ifaces[interface_id].tsresol;
		break;
	}
#ifdef _WIN32
	/*
	 * tv_sec and tv_used in the Windows struct timeval are both
	 * longs.
	 */
	hdr->ts.tv_sec = (long)sec;
	hdr->ts.tv_usec = (long)frac;
#else
	/*
	 * tv_sec in the UN*X struct timeval is a time_t; tv_usec is
	 * suseconds_t in UN*Xes that work the way the current Single
	 * UNIX Standard specify - but not all older UN*Xes necessarily
	 * support that type, so just cast to int.
	 */
	hdr->ts.tv_sec = (time_t)sec;
	hdr->ts.tv_usec = (int)frac;
#endif

	/*
	 * Get a pointer to the packet data.
	 */
	*data = get_from_block_data(&cursor, hdr->caplen, p->errbuf);
	if (*data == NULL)
		return (-1);

	if (p->swapped)
		swap_pseudo_headers(p->linktype, hdr, *data);

	return (0);
}
