sec_parse_public_key(STREAM s, uint8 * modulus, uint8 * exponent)
{
	uint32 magic, modulus_len;

	in_uint32_le(s, magic);
	if (magic != SEC_RSA_MAGIC)
	{
		logger(Protocol, Error, "sec_parse_public_key(), magic (0x%x) != SEC_RSA_MAGIC",
		       magic);
		return False;
	}

	in_uint32_le(s, modulus_len);
	modulus_len -= SEC_PADDING_SIZE;
	if ((modulus_len < SEC_MODULUS_SIZE) || (modulus_len > SEC_MAX_MODULUS_SIZE))
	{
		logger(Protocol, Error,
		       "sec_parse_public_key(), invalid public key size (%u bits) from server",
		       modulus_len * 8);
		return False;
	}

	in_uint8s(s, 8);	/* modulus_bits, unknown */
	in_uint8a(s, exponent, SEC_EXPONENT_SIZE);
	in_uint8a(s, modulus, modulus_len);
	in_uint8s(s, SEC_PADDING_SIZE);
	g_server_public_key_len = modulus_len;

	return s_check(s);
}
