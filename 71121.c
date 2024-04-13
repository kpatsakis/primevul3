build_edi(krb5_context context,
	  hx509_context hx509ctx,
	  hx509_certs certs,
	  ExternalPrincipalIdentifiers *ids)
{
    return hx509_certs_iter_f(hx509ctx, certs, cert2epi, ids);
}
