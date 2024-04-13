rdp_ts_in_share_control_header(STREAM s, uint8 * type, uint16 * length)
{
	uint16 pdu_type;
	uint16 pdu_source;

	UNUSED(pdu_source);

	in_uint16_le(s, *length);	/* totalLength */

	/* If the totalLength field equals 0x8000, then the Share
	   Control Header and any data that follows MAY be interpreted
	   as a T.128 FlowPDU as described in [T128] section 8.5 (the
	   ASN.1 structure definition is detailed in [T128] section
	   9.1) and MUST be ignored.
	 */
	if (*length == 0x8000)
	{
		/* skip over this message in stream */
		g_next_packet += 8;
		return False;
	}

	in_uint16_le(s, pdu_type);	/* pduType */
	in_uint16(s, pdu_source);	/* pduSource */

	*type = pdu_type & 0xf;

	return True;
}
