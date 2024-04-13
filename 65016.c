static int x509_v3_subject_alt_name(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    if ((offset = asn1_is_subject_alt_name(cert, offset)) > 0)
    {
        x509_ctx->subject_alt_name_present = true;
        x509_ctx->subject_alt_name_is_critical = 
                        asn1_is_critical_ext(cert, &offset);

        if (asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) > 0)
        {
            int altlen;

            if ((altlen = asn1_next_obj(cert, &offset, ASN1_SEQUENCE)) > 0)
            {
                int endalt = offset + altlen;
                int totalnames = 0;

                while (offset < endalt)
                {
                    int type = cert[offset++];
                    int dnslen = get_asn1_length(cert, &offset);

                    if (type == ASN1_CONTEXT_DNSNAME)
                    {
                        x509_ctx->subject_alt_dnsnames = (char**)
                                realloc(x509_ctx->subject_alt_dnsnames, 
                                   (totalnames + 2) * sizeof(char*));
                        x509_ctx->subject_alt_dnsnames[totalnames] = 
                                (char*)malloc(dnslen + 1);
                        x509_ctx->subject_alt_dnsnames[totalnames+1] = NULL;
                        memcpy(x509_ctx->subject_alt_dnsnames[totalnames], 
                                cert + offset, dnslen);
                        x509_ctx->subject_alt_dnsnames[totalnames][dnslen] = 0;
                        totalnames++;
                    }

                    offset += dnslen;
                }
            }
        }
    }

    return X509_OK;
}
