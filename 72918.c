sec_process_mcs_data(STREAM s)
{
	uint16 tag, length;
	uint8 *next_tag;
	uint8 len;

	in_uint8s(s, 21);	/* header (T.124 ConferenceCreateResponse) */
	in_uint8(s, len);
	if (len & 0x80)
		in_uint8(s, len);
	logger(Protocol, Debug, "%s()", __func__);

	while (s->p < s->end)
	{
		in_uint16_le(s, tag);
		in_uint16_le(s, length);

		if (length <= 4)
			return;

		next_tag = s->p + length - 4;

		switch (tag)
		{
			case SEC_TAG_SRV_INFO:
				logger(Protocol, Debug, "%s(), SEC_TAG_SRV_INFO", __func__);
				sec_process_srv_info(s);
				break;

			case SEC_TAG_SRV_CRYPT:
				logger(Protocol, Debug, "%s(), SEC_TAG_SRV_CRYPT", __func__);
				sec_process_crypt_info(s);
				break;

			case SEC_TAG_SRV_CHANNELS:
				logger(Protocol, Debug, "%s(), SEC_TAG_SRV_CHANNELS", __func__);
				/* FIXME: We should parse this information and
				   use it to map RDP5 channels to MCS 
				   channels */
				break;

			default:
				logger(Protocol, Warning, "Unhandled response tag 0x%x", tag);
		}

		s->p = next_tag;
	}
}
