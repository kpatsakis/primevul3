static int pgpPrtPkt(struct pgpPkt *p, pgpDigParams _digp)
{
    int rc = 0;

    switch (p->tag) {
    case PGPTAG_SIGNATURE:
	rc = pgpPrtSig(p->tag, p->body, p->blen, _digp);
	break;
    case PGPTAG_PUBLIC_KEY:
	/* Get the public key Key ID. */
	if (!getKeyID(p->body, p->blen, _digp->signid))
	    _digp->saved |= PGPDIG_SAVED_ID;
	else
	    memset(_digp->signid, 0, sizeof(_digp->signid));
	rc = pgpPrtKey(p->tag, p->body, p->blen, _digp);
	break;
    case PGPTAG_USER_ID:
	rc = pgpPrtUserID(p->tag, p->body, p->blen, _digp);
	break;
    case PGPTAG_RESERVED:
	rc = -1;
	break;
    case PGPTAG_COMMENT:
    case PGPTAG_COMMENT_OLD:
    case PGPTAG_PUBLIC_SUBKEY:
    case PGPTAG_SECRET_KEY:
    case PGPTAG_SECRET_SUBKEY:
    case PGPTAG_PUBLIC_SESSION_KEY:
    case PGPTAG_SYMMETRIC_SESSION_KEY:
    case PGPTAG_COMPRESSED_DATA:
    case PGPTAG_SYMMETRIC_DATA:
    case PGPTAG_MARKER:
    case PGPTAG_LITERAL_DATA:
    case PGPTAG_TRUST:
    case PGPTAG_PHOTOID:
    case PGPTAG_ENCRYPTED_MDC:
    case PGPTAG_MDC:
    case PGPTAG_PRIVATE_60:
    case PGPTAG_PRIVATE_62:
    case PGPTAG_CONTROL:
    default:
	pgpPrtVal("", pgpTagTbl, p->tag);
	pgpPrtHex("", p->body, p->blen);
	pgpPrtNL();
	break;
    }

    return rc;
}