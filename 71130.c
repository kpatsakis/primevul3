integer_to_BN(krb5_context context, const char *field, const heim_integer *f)
{
    BIGNUM *bn;

    bn = BN_bin2bn((const unsigned char *)f->data, f->length, NULL);
    if (bn == NULL) {
	krb5_set_error_message(context, ENOMEM,
			       N_("PKINIT: parsing BN failed %s", ""), field);
	return NULL;
    }
    BN_set_negative(bn, f->negative);
    return bn;
}
