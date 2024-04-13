static int x509_v3_basic_constraints(const uint8_t *cert, int offset, 
        X509_CTX *x509_ctx)
{
    int ret = X509_OK;
    int lenSeq = 0;

    if ((offset = asn1_is_basic_constraints(cert, offset)) == 0)
        goto end_contraints;

    x509_ctx->basic_constraint_present = true;
    x509_ctx->basic_constraint_is_critical = 
                    asn1_is_critical_ext(cert, &offset);

    /* Assign Defaults in case not specified
    basic_constraint_cA will already by zero by virtue of the calloc */
    x509_ctx->basic_constraint_cA = 0;
    /* basic_constraint_pathLenConstraint is unlimited by default. 
    10000 is just a large number (limits.h is not already included) */
    x509_ctx->basic_constraint_pathLenConstraint = 10000;
    
    if ((asn1_next_obj(cert, &offset, ASN1_OCTET_STRING) < 0) ||
            ((lenSeq = asn1_next_obj(cert, &offset, ASN1_SEQUENCE)) < 0))
    {
        ret = X509_NOT_OK;       
    }
    
    /* If the Sequence Length is greater than zero, 
    continue with the basic_constraint_cA */
    if ((lenSeq>0)&&(asn1_get_bool(cert, &offset, 
            &x509_ctx->basic_constraint_cA) < 0))
    {
        ret = X509_NOT_OK;
    }
    
    /* If the Sequence Length is greater than 3, it has more content than 
    the basic_constraint_cA bool, so grab the pathLenConstraint */
    if ((lenSeq>3) && (asn1_get_int(cert, &offset, 
            &x509_ctx->basic_constraint_pathLenConstraint) < 0))
    {
        ret = X509_NOT_OK;
    }

end_contraints:
    return ret;
}
