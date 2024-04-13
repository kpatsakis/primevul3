make_pa_enc_timestamp(krb5_context context, METHOD_DATA *md,
		      krb5_enctype etype, krb5_keyblock *key)
{
    PA_ENC_TS_ENC p;
    unsigned char *buf;
    size_t buf_size;
    size_t len = 0;
    EncryptedData encdata;
    krb5_error_code ret;
    int32_t usec;
    int usec2;
    krb5_crypto crypto;

    krb5_us_timeofday (context, &p.patimestamp, &usec);
    usec2         = usec;
    p.pausec      = &usec2;

    ASN1_MALLOC_ENCODE(PA_ENC_TS_ENC, buf, buf_size, &p, &len, ret);
    if (ret)
	return ret;
    if(buf_size != len)
	krb5_abortx(context, "internal error in ASN.1 encoder");

    ret = krb5_crypto_init(context, key, 0, &crypto);
    if (ret) {
	free(buf);
	return ret;
    }
    ret = krb5_encrypt_EncryptedData(context,
				     crypto,
				     KRB5_KU_PA_ENC_TIMESTAMP,
				     buf,
				     len,
				     0,
				     &encdata);
    free(buf);
    krb5_crypto_destroy(context, crypto);
    if (ret)
	return ret;

    ASN1_MALLOC_ENCODE(EncryptedData, buf, buf_size, &encdata, &len, ret);
    free_EncryptedData(&encdata);
    if (ret)
	return ret;
    if(buf_size != len)
	krb5_abortx(context, "internal error in ASN.1 encoder");

    ret = krb5_padata_add(context, md, KRB5_PADATA_ENC_TIMESTAMP, buf, len);
    if (ret)
	free(buf);
    return ret;
}
