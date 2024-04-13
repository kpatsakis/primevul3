_krb5_pk_cert_free(struct krb5_pk_cert *cert)
{
    if (cert->cert) {
	hx509_cert_free(cert->cert);
    }
    free(cert);
}
