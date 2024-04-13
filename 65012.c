int x509_new(const uint8_t *cert, int *len, X509_CTX **ctx)
{
    int begin_tbs, end_tbs, begin_spki, end_spki;
    int ret = X509_NOT_OK, offset = 0, cert_size = 0;
    int version = 0;
    X509_CTX *x509_ctx;
#ifdef CONFIG_SSL_CERT_VERIFICATION /* only care if doing verification */
    BI_CTX *bi_ctx;
#endif

    *ctx = (X509_CTX *)calloc(1, sizeof(X509_CTX));
    x509_ctx = *ctx;

    /* get the certificate size */
    asn1_skip_obj(cert, &cert_size, ASN1_SEQUENCE); 

    if (asn1_next_obj(cert, &offset, ASN1_SEQUENCE) < 0)
        goto end_cert;

    begin_tbs = offset;         /* start of the tbs */
    end_tbs = begin_tbs;        /* work out the end of the tbs */
    asn1_skip_obj(cert, &end_tbs, ASN1_SEQUENCE);

    if (asn1_next_obj(cert, &offset, ASN1_SEQUENCE) < 0)
        goto end_cert;

    /* optional version */
    if (cert[offset] == ASN1_EXPLICIT_TAG && 
            asn1_version(cert, &offset, &version) == X509_NOT_OK)
        goto end_cert;

    if (asn1_skip_obj(cert, &offset, ASN1_INTEGER) || /* serial number */ 
            asn1_next_obj(cert, &offset, ASN1_SEQUENCE) < 0)
        goto end_cert;

    /* make sure the signature is ok */
    if (asn1_signature_type(cert, &offset, x509_ctx))
    {
        ret = X509_VFY_ERROR_UNSUPPORTED_DIGEST;
        goto end_cert;
    }

    if (asn1_name(cert, &offset, x509_ctx->ca_cert_dn) || 
            asn1_validity(cert, &offset, x509_ctx) ||
            asn1_name(cert, &offset, x509_ctx->cert_dn))
    {
        goto end_cert;
    }
    begin_spki = offset;
    if (asn1_public_key(cert, &offset, x509_ctx))
        goto end_cert;
    end_spki = offset;

    x509_ctx->fingerprint = malloc(SHA1_SIZE);
    SHA1_CTX sha_fp_ctx;
    SHA1_Init(&sha_fp_ctx);
    SHA1_Update(&sha_fp_ctx, &cert[0], cert_size);
    SHA1_Final(x509_ctx->fingerprint, &sha_fp_ctx);

    x509_ctx->spki_sha256 = malloc(SHA256_SIZE);
    SHA256_CTX spki_hash_ctx;
    SHA256_Init(&spki_hash_ctx);
    SHA256_Update(&spki_hash_ctx, &cert[begin_spki], end_spki-begin_spki);
    SHA256_Final(x509_ctx->spki_sha256, &spki_hash_ctx);

#ifdef CONFIG_SSL_CERT_VERIFICATION /* only care if doing verification */
    bi_ctx = x509_ctx->rsa_ctx->bi_ctx;

    /* use the appropriate signature algorithm */
    switch (x509_ctx->sig_type)
    {
        case SIG_TYPE_MD5:
        {
            MD5_CTX md5_ctx;
            uint8_t md5_dgst[MD5_SIZE];
            MD5_Init(&md5_ctx);
            MD5_Update(&md5_ctx, &cert[begin_tbs], end_tbs-begin_tbs);
            MD5_Final(md5_dgst, &md5_ctx);
            x509_ctx->digest = bi_import(bi_ctx, md5_dgst, MD5_SIZE);
        }
            break;

        case SIG_TYPE_SHA1:
        {
            SHA1_CTX sha_ctx;
            uint8_t sha_dgst[SHA1_SIZE];
            SHA1_Init(&sha_ctx);
            SHA1_Update(&sha_ctx, &cert[begin_tbs], end_tbs-begin_tbs);
            SHA1_Final(sha_dgst, &sha_ctx);
            x509_ctx->digest = bi_import(bi_ctx, sha_dgst, SHA1_SIZE);
        }
            break;

        case SIG_TYPE_SHA256:
        {
            SHA256_CTX sha256_ctx;
            uint8_t sha256_dgst[SHA256_SIZE];
            SHA256_Init(&sha256_ctx);
            SHA256_Update(&sha256_ctx, &cert[begin_tbs], end_tbs-begin_tbs);
            SHA256_Final(sha256_dgst, &sha256_ctx);
            x509_ctx->digest = bi_import(bi_ctx, sha256_dgst, SHA256_SIZE);
        }
            break;

        case SIG_TYPE_SHA384:
        {
            SHA384_CTX sha384_ctx;
            uint8_t sha384_dgst[SHA384_SIZE];
            SHA384_Init(&sha384_ctx);
            SHA384_Update(&sha384_ctx, &cert[begin_tbs], end_tbs-begin_tbs);
            SHA384_Final(sha384_dgst, &sha384_ctx);
            x509_ctx->digest = bi_import(bi_ctx, sha384_dgst, SHA384_SIZE);
        }
            break;

        case SIG_TYPE_SHA512:
        {
            SHA512_CTX sha512_ctx;
            uint8_t sha512_dgst[SHA512_SIZE];
            SHA512_Init(&sha512_ctx);
            SHA512_Update(&sha512_ctx, &cert[begin_tbs], end_tbs-begin_tbs);
            SHA512_Final(sha512_dgst, &sha512_ctx);
            x509_ctx->digest = bi_import(bi_ctx, sha512_dgst, SHA512_SIZE);
        }
            break;
    }

    if (version == 2 && asn1_next_obj(cert, &offset, ASN1_V3_DATA) > 0)
    {
        x509_v3_subject_alt_name(cert, offset, x509_ctx);
        x509_v3_basic_constraints(cert, offset, x509_ctx);
        x509_v3_key_usage(cert, offset, x509_ctx);
    }

    offset = end_tbs;   /* skip the rest of v3 data */
    if (asn1_skip_obj(cert, &offset, ASN1_SEQUENCE) || 
            asn1_signature(cert, &offset, x509_ctx))
        goto end_cert;

    /* Saves a few bytes of memory */
    bi_clear_cache(bi_ctx);
#endif
    ret = X509_OK;
end_cert:
    if (len)
    {
        *len = cert_size;
    }

    if (ret)
    {
#ifdef CONFIG_SSL_FULL_MODE
        char buff[64];
        printf("Error: Invalid X509 ASN.1 file (%s)\n",
                        x509_display_error(ret, buff));
#endif
        x509_free(x509_ctx);
        *ctx = NULL;
    }

    return ret;
}
