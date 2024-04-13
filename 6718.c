pgpDig pgpFreeDig(pgpDig dig)
{
    if (dig != NULL) {

	/* DUmp the signature/pubkey data. */
	pgpCleanDig(dig);
	dig = _free(dig);
    }
    return dig;
}