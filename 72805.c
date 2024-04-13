process_data_pdu(STREAM s, uint32 * ext_disc_reason)
{
	uint8 data_pdu_type;
	uint8 ctype;
	uint16 clen;
	uint32 len;

	uint32 roff, rlen;

	struct stream *ns = &(g_mppc_dict.ns);

	in_uint8s(s, 6);	/* shareid, pad, streamid */
	in_uint16_le(s, len);
	in_uint8(s, data_pdu_type);
	in_uint8(s, ctype);
	in_uint16_le(s, clen);
	clen -= 18;

	if (ctype & RDP_MPPC_COMPRESSED)
	{
		if (len > RDP_MPPC_DICT_SIZE)
			logger(Protocol, Error,
			       "process_data_pdu(), error decompressed packet size exceeds max");
		if (mppc_expand(s->p, clen, ctype, &roff, &rlen) == -1)
			logger(Protocol, Error,
			       "process_data_pdu(), error while decompressing packet");

		/* len -= 18; */

		/* allocate memory and copy the uncompressed data into the temporary stream */
		ns->data = (uint8 *) xrealloc(ns->data, rlen);

		memcpy((ns->data), (unsigned char *) (g_mppc_dict.hist + roff), rlen);

		ns->size = rlen;
		ns->end = (ns->data + ns->size);
		ns->p = ns->data;
		ns->rdp_hdr = ns->p;

		s = ns;
	}

	switch (data_pdu_type)
	{
		case RDP_DATA_PDU_UPDATE:
			process_update_pdu(s);
			break;

		case RDP_DATA_PDU_CONTROL:
			logger(Protocol, Debug, "process_data_pdu(), received Control PDU");
			break;

		case RDP_DATA_PDU_SYNCHRONISE:
			logger(Protocol, Debug, "process_data_pdu(), received Sync PDU");
			break;

		case RDP_DATA_PDU_POINTER:
			process_pointer_pdu(s);
			break;

		case RDP_DATA_PDU_BELL:
			ui_bell();
			break;

		case RDP_DATA_PDU_LOGON:
			logger(Protocol, Debug, "process_data_pdu(), received Logon PDU");
			/* User logged on */
			process_pdu_logon(s);
			break;

		case RDP_DATA_PDU_SET_ERROR_INFO:
			process_ts_set_error_info_pdu(s, ext_disc_reason);

			/* We used to return true and disconnect immediately here, but
			 * Windows Vista sends a disconnect PDU with reason 0 when
			 * reconnecting to a disconnected session, and MSTSC doesn't
			 * drop the connection.  I think we should just save the status.
			 */
			break;

		case RDP_DATA_PDU_AUTORECONNECT_STATUS:
			logger(Protocol, Warning,
			       "process_data_pdu(), automatic reconnect using cookie, failed");
			break;

		default:
			logger(Protocol, Warning, "process_data_pdu(), unhandled data PDU type %d",
			       data_pdu_type);
	}
	return False;
}
