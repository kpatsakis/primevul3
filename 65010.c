const char * x509_display_error(int error, char *buff)
{
    switch (error)
    {
        case X509_OK:
            strcpy_P(buff, "Certificate verify successful");
            return buff;

        case X509_NOT_OK:
            strcpy_P(buff, "X509 not ok");
            return buff;

        case X509_VFY_ERROR_NO_TRUSTED_CERT:
            strcpy_P(buff, "No trusted cert is available");
            return buff;

        case X509_VFY_ERROR_BAD_SIGNATURE:
            strcpy_P(buff, "Bad signature");
            return buff;

        case X509_VFY_ERROR_NOT_YET_VALID:
            strcpy_P(buff, "Cert is not yet valid");
            return buff;

        case X509_VFY_ERROR_EXPIRED:
            strcpy_P(buff, "Cert has expired");
            return buff;

        case X509_VFY_ERROR_SELF_SIGNED:
            strcpy_P(buff, "Cert is self-signed");
            return buff;

        case X509_VFY_ERROR_INVALID_CHAIN:
            strcpy_P(buff, "Chain is invalid (check order of certs)");
            return buff;

        case X509_VFY_ERROR_UNSUPPORTED_DIGEST:
            strcpy_P(buff, "Unsupported digest");
            return buff;

        case X509_INVALID_PRIV_KEY:
            strcpy_P(buff, "Invalid private key");
            return buff;

        case X509_VFY_ERROR_BASIC_CONSTRAINT:
            strcpy_P(buff, "Basic constraint invalid");
            return buff;

        default:
            strcpy_P(buff, "Unknown");
            return buff;
    }
}
