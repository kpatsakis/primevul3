void x509_print(const X509_CTX *cert, CA_CERT_CTX *ca_cert_ctx) 
{
    if (cert == NULL)
        return;

    char not_part_of_cert[30];
    strcpy_P(not_part_of_cert, "<Not Part Of Certificate>");
    char critical[16];
    strcpy_P(critical, "critical, ");

    printf("=== CERTIFICATE ISSUED TO ===\n");
    printf("Common Name (CN):\t\t");
    printf("%s\n", cert->cert_dn[X509_COMMON_NAME] ?
                    cert->cert_dn[X509_COMMON_NAME] : not_part_of_cert);

    printf("Organization (O):\t\t");
    printf("%s\n", cert->cert_dn[X509_ORGANIZATION] ?
        cert->cert_dn[X509_ORGANIZATION] : not_part_of_cert);

    if (cert->cert_dn[X509_ORGANIZATIONAL_UNIT]) 
    {
        printf("Organizational Unit (OU):\t");
        printf("%s\n", cert->cert_dn[X509_ORGANIZATIONAL_UNIT]);
    }

    if (cert->cert_dn[X509_LOCATION]) 
    {
        printf("Location (L):\t\t\t");
        printf("%s\n", cert->cert_dn[X509_LOCATION]);
    }

    if (cert->cert_dn[X509_COUNTRY]) 
    {
        printf("Country (C):\t\t\t");
        printf("%s\n", cert->cert_dn[X509_COUNTRY]);
    }

    if (cert->cert_dn[X509_STATE]) 
    {
        printf("State (ST):\t\t\t");
        printf("%s\n", cert->cert_dn[X509_STATE]);
    }

    if (cert->basic_constraint_present)
    {
        printf("Basic Constraints:\t\t%sCA:%s, pathlen:%d\n",
                cert->basic_constraint_is_critical ? 
                    critical : "",
                cert->basic_constraint_cA? "TRUE" : "FALSE",
                cert->basic_constraint_pathLenConstraint);
    }

    if (cert->key_usage_present)
    {
        printf("Key Usage:\t\t\t%s", cert->key_usage_is_critical ? 
                    critical : "");
        bool has_started = false;

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_DIGITAL_SIGNATURE))
        {
            printf("Digital Signature");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_NON_REPUDIATION))
        {
            if (has_started)
                printf(", ");

            printf("Non Repudiation");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_ENCIPHERMENT))
        {
            if (has_started)
                printf(", ");

            printf("Key Encipherment");
            has_started = true;
        }
        
        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_DATA_ENCIPHERMENT))
        {
            if (has_started)
                printf(", ");

            printf("Data Encipherment");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_AGREEMENT))
        {
            if (has_started)
                printf(", ");

            printf("Key Agreement");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_KEY_CERT_SIGN))
        {
            if (has_started)
                printf(", ");

            printf("Key Cert Sign");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_CRL_SIGN))
        {
            if (has_started)
                printf(", ");

            printf("CRL Sign");
            has_started = true;
        }
       
        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_ENCIPHER_ONLY))
        {
            if (has_started)
                printf(", ");

            printf("Encipher Only");
            has_started = true;
        }

        if (IS_SET_KEY_USAGE_FLAG(cert, KEY_USAGE_DECIPHER_ONLY))
        {
            if (has_started)
                printf(", ");

            printf("Decipher Only");
            has_started = true;
        }

        printf("\n");
    }

    if (cert->subject_alt_name_present)
    {
        printf("Subject Alt Name:\t\t%s", cert->subject_alt_name_is_critical 
                ?  critical : "");
        if (cert->subject_alt_dnsnames)
        {
            int i = 0;

            while (cert->subject_alt_dnsnames[i])
                printf("%s ", cert->subject_alt_dnsnames[i++]);
        }
        printf("\n");

    }

    printf("=== CERTIFICATE ISSUED BY ===\n");
    printf("Common Name (CN):\t\t");
    printf("%s\n", cert->ca_cert_dn[X509_COMMON_NAME] ?
                    cert->ca_cert_dn[X509_COMMON_NAME] : not_part_of_cert);

    printf("Organization (O):\t\t");
    printf("%s\n", cert->ca_cert_dn[X509_ORGANIZATION] ?
        cert->ca_cert_dn[X509_ORGANIZATION] : not_part_of_cert);

    if (cert->ca_cert_dn[X509_ORGANIZATIONAL_UNIT]) 
    {
        printf("Organizational Unit (OU):\t");
        printf("%s\n", cert->ca_cert_dn[X509_ORGANIZATIONAL_UNIT]);
    }

    if (cert->ca_cert_dn[X509_LOCATION]) 
    {
        printf("Location (L):\t\t\t");
        printf("%s\n", cert->ca_cert_dn[X509_LOCATION]);
    }

    if (cert->ca_cert_dn[X509_COUNTRY]) 
    {
        printf("Country (C):\t\t\t");
        printf("%s\n", cert->ca_cert_dn[X509_COUNTRY]);
    }

    if (cert->ca_cert_dn[X509_STATE]) 
    {
        printf("State (ST):\t\t\t");
        printf("%s\n", cert->ca_cert_dn[X509_STATE]);
    }

    printf("Not Before:\t\t\t%s", ctime(&cert->not_before));
    printf("Not After:\t\t\t%s", ctime(&cert->not_after));
    printf("RSA bitsize:\t\t\t%d\n", cert->rsa_ctx->num_octets*8);
    printf("Sig Type:\t\t\t");
    switch (cert->sig_type)
    {
        case SIG_TYPE_MD5:
            printf("MD5\n");
            break;
        case SIG_TYPE_SHA1:
            printf("SHA1\n");
            break;
        case SIG_TYPE_SHA256:
            printf("SHA256\n");
            break;
        case SIG_TYPE_SHA384:
            printf("SHA384\n");
            break;
        case SIG_TYPE_SHA512:
            printf("SHA512\n");
            break;
        default:
            printf("Unrecognized: %d\n", cert->sig_type);
            break;
    }

    if (ca_cert_ctx)
    {
        int pathLenConstraint = 0;
        char buff[64];
        printf("Verify:\t\t\t\t%s\n",
                x509_display_error(x509_verify(ca_cert_ctx, cert,
                        &pathLenConstraint), buff));
    }

#if 0
    print_blob("Signature", cert->signature, cert->sig_len);
    bi_print("Modulus", cert->rsa_ctx->m);
    bi_print("Pub Exp", cert->rsa_ctx->e);
#endif

    if (ca_cert_ctx)
    {
        x509_print(cert->next, ca_cert_ctx);
    }

    TTY_FLUSH();
}
