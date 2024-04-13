char *pgpIdentItem(pgpDigParams digp)
{
    char *id = NULL;
    if (digp) {
	
	char *signid = pgpHexStr(digp->signid+4, sizeof(digp->signid)-4);
	rasprintf(&id, _("V%d %s/%s %s, key ID %s"),
			digp->version,
			pgpValStr(pgpPubkeyTbl, digp->pubkey_algo),
			pgpValStr(pgpHashTbl, digp->hash_algo),
			pgpValStr(pgpTagTbl, digp->tag),
			signid);
	free(signid);
    } else {
	id = xstrdup(_("(none)"));
    }
    return id;
}