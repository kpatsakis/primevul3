int pgpDigParamsCmp(pgpDigParams p1, pgpDigParams p2)
{
    int rc = 1; /* assume different, eg if either is NULL */
    if (p1 && p2) {
	/* XXX Should we compare something else too? */
	if (p1->tag != p2->tag)
	    goto exit;
	if (p1->hash_algo != p2->hash_algo)
	    goto exit;
	if (p1->pubkey_algo != p2->pubkey_algo)
	    goto exit;
	if (p1->version != p2->version)
	    goto exit;
	if (p1->sigtype != p2->sigtype)
	    goto exit;
	if (memcmp(p1->signid, p2->signid, sizeof(p1->signid)) != 0)
	    goto exit;
	if (p1->userid && p2->userid && strcmp(p1->userid, p2->userid) != 0)
	    goto exit;

	/* Parameters match ... at least for our purposes */
	rc = 0;
    }
exit:
    return rc;
}