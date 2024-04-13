void x509_free(X509_CTX *x509_ctx)
{
    X509_CTX *next;
    int i;

    if (x509_ctx == NULL)       /* if already null, then don't bother */
        return;

    for (i = 0; i < X509_NUM_DN_TYPES; i++)
    {
        free(x509_ctx->ca_cert_dn[i]);
        free(x509_ctx->cert_dn[i]);
    }

    free(x509_ctx->signature);

#ifdef CONFIG_SSL_CERT_VERIFICATION 
    if (x509_ctx->digest)
    {
        bi_free(x509_ctx->rsa_ctx->bi_ctx, x509_ctx->digest);
    }

    if (x509_ctx->fingerprint)
    {
        free(x509_ctx->fingerprint);
    }

    if (x509_ctx->spki_sha256)
    {
        free(x509_ctx->spki_sha256);
    }

    if (x509_ctx->subject_alt_dnsnames)
    {
        for (i = 0; x509_ctx->subject_alt_dnsnames[i]; ++i)
            free(x509_ctx->subject_alt_dnsnames[i]);

        free(x509_ctx->subject_alt_dnsnames);
    }
#endif

    RSA_free(x509_ctx->rsa_ctx);
    next = x509_ctx->next;
    free(x509_ctx);
    x509_free(next);        /* clear the chain */
 }
