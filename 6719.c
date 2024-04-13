pgpDigParams pgpDigParamsFree(pgpDigParams digp)
{
    if (digp) {
	pgpDigAlgFree(digp->alg);
	free(digp->userid);
	free(digp->hash);
	memset(digp, 0, sizeof(*digp));
	free(digp);
    }
    return NULL;
}