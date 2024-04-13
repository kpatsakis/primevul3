sc_asn1_sig_value_sequence_to_rs(struct sc_context *ctx, const unsigned char *in, size_t inlen,
		unsigned char *buf, size_t buflen)
{
	struct sc_asn1_entry asn1_sig_value[C_ASN1_SIG_VALUE_SIZE];
	struct sc_asn1_entry asn1_sig_value_coefficients[C_ASN1_SIG_VALUE_COEFFICIENTS_SIZE];
	unsigned char *r = NULL, *s = NULL;
	size_t r_len, s_len, halflen = buflen/2;
	int rv;

	LOG_FUNC_CALLED(ctx);
	if (!buf || !buflen)
		LOG_FUNC_RETURN(ctx, SC_ERROR_INVALID_ARGUMENTS);

	sc_copy_asn1_entry(c_asn1_sig_value, asn1_sig_value);
	sc_format_asn1_entry(asn1_sig_value + 0, asn1_sig_value_coefficients, NULL, 0);

	sc_copy_asn1_entry(c_asn1_sig_value_coefficients, asn1_sig_value_coefficients);
	sc_format_asn1_entry(asn1_sig_value_coefficients + 0, &r, &r_len, 0);
	sc_format_asn1_entry(asn1_sig_value_coefficients + 1, &s, &s_len, 0);

	rv = sc_asn1_decode(ctx, asn1_sig_value, in, inlen, NULL, NULL);
	LOG_TEST_GOTO_ERR(ctx, rv, "ASN.1 decoding ECDSA-Sig-Value failed");

	if (halflen < r_len || halflen < s_len)   {
		rv = SC_ERROR_BUFFER_TOO_SMALL;
		goto err;
	}

	memset(buf, 0, buflen);
	memcpy(buf + (halflen - r_len), r, r_len);
	memcpy(buf + (buflen - s_len), s, s_len);

	sc_log(ctx, "r(%"SC_FORMAT_LEN_SIZE_T"u): %s", halflen,
	       sc_dump_hex(buf, halflen));
	sc_log(ctx, "s(%"SC_FORMAT_LEN_SIZE_T"u): %s", halflen,
	       sc_dump_hex(buf + halflen, halflen));

	rv = SC_SUCCESS;
err:
	free(r);
	free(s);

	LOG_FUNC_RETURN(ctx, rv);
}
