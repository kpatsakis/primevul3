void pgpCleanDig(pgpDig dig)
{
    if (dig != NULL) {
	pgpDigParamsFree(dig->signature);
	pgpDigParamsFree(dig->pubkey);
	memset(dig, 0, sizeof(*dig));
    }
    return;
}