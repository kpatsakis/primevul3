print_lockingandx(netdissect_options *ndo,
                  const u_char *words, const u_char *data, const u_char *buf _U_, const u_char *maxbuf)
{
    u_int wct, bcc;
    const u_char *maxwords;
    const char *f1 = NULL, *f2 = NULL;

    ND_TCHECK(words[0]);
    wct = words[0];
    if (request) {
	f1 = "Com2=[w]\nOff2=[d]\nHandle=[d]\nLockType=[w]\nTimeOut=[D]\nUnlockCount=[d]\nLockCount=[d]\n";
	ND_TCHECK(words[7]);
	if (words[7] & 0x10)
	    f2 = "*Process=[d]\n[P2]Offset=[M]\nLength=[M]\n";
	else
	    f2 = "*Process=[d]\nOffset=[D]\nLength=[D]\n";
    } else {
	f1 = "Com2=[w]\nOff2=[d]\n";
    }

    maxwords = min(words + 1 + wct * 2, maxbuf);
    if (wct)
	smb_fdata(ndo, words + 1, f1, maxwords, unicodestr);

    ND_TCHECK2(*data, 2);
    bcc = EXTRACT_LE_16BITS(data);
    ND_PRINT((ndo, "smb_bcc=%u\n", bcc));
    if (bcc > 0) {
	if (f2)
	    smb_fdata(ndo, data + 2, f2, min(data + 2 + EXTRACT_LE_16BITS(data),
		maxbuf), unicodestr);
	else
	    smb_print_data(ndo, data + 2, min(EXTRACT_LE_16BITS(data), PTR_DIFF(maxbuf, data + 2)));
    }
    return;
trunc:
    ND_PRINT((ndo, "%s", tstr));
}
