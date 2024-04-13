rpmRC pgpVerifySig(pgpDig dig, DIGEST_CTX hashctx)
{
    if (dig == NULL || hashctx == NULL)
	return RPMRC_FAIL;

    return pgpVerifySignature(pgpDigGetParams(dig, PGPTAG_PUBLIC_KEY),
			      pgpDigGetParams(dig, PGPTAG_SIGNATURE), hashctx);
}