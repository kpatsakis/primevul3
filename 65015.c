static int x509_v3_key_usage(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    int ret = X509_OK;

    if ((offset = asn1_is_key_usage(cert, offset)) == 0)
        goto end_key_usage;

    x509_ctx->key_usage_present = true;
    x509_ctx->key_usage_is_critical = asn1_is_critical_ext(cert, &offset);

    if (asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) < 0 ||
            asn1_get_bit_string_as_int(cert, &offset, &x509_ctx->key_usage))
    {
        ret = X509_NOT_OK;       
    }

end_key_usage:
    return ret;
}
