BN_to_integer(krb5_context context, BIGNUM *bn, heim_integer *integer)
{
    integer->length = BN_num_bytes(bn);
    integer->data = malloc(integer->length);
    if (integer->data == NULL) {
	krb5_clear_error_message(context);
	return ENOMEM;
    }
    BN_bn2bin(bn, integer->data);
    integer->negative = BN_is_negative(bn);
    return 0;
}
