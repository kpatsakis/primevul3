print_smb(netdissect_options *ndo,
          const u_char *buf, const u_char *maxbuf)
{
    uint16_t flags2;
    int nterrcodes;
    int command;
    uint32_t nterror;
    const u_char *words, *maxwords, *data;
    const struct smbfns *fn;
    const char *fmt_smbheader =
        "[P4]SMB Command   =  [B]\nError class   =  [BP1]\nError code    =  [d]\nFlags1        =  [B]\nFlags2        =  [B][P13]\nTree ID       =  [d]\nProc ID       =  [d]\nUID           =  [d]\nMID           =  [d]\nWord Count    =  [b]\n";
    int smboffset;

    ND_TCHECK(buf[9]);
    request = (buf[9] & 0x80) ? 0 : 1;
    startbuf = buf;

    command = buf[4];

    fn = smbfind(command, smb_fns);

    if (ndo->ndo_vflag > 1)
	ND_PRINT((ndo, "\n"));

    ND_PRINT((ndo, "SMB PACKET: %s (%s)\n", fn->name, request ? "REQUEST" : "REPLY"));

    if (ndo->ndo_vflag < 2)
	return;

    ND_TCHECK_16BITS(&buf[10]);
    flags2 = EXTRACT_LE_16BITS(&buf[10]);
    unicodestr = flags2 & 0x8000;
    nterrcodes = flags2 & 0x4000;

    /* print out the header */
    smb_fdata(ndo, buf, fmt_smbheader, buf + 33, unicodestr);

    if (nterrcodes) {
    	nterror = EXTRACT_LE_32BITS(&buf[5]);
	if (nterror)
	    ND_PRINT((ndo, "NTError = %s\n", nt_errstr(nterror)));
    } else {
	if (buf[5])
	    ND_PRINT((ndo, "SMBError = %s\n", smb_errstr(buf[5], EXTRACT_LE_16BITS(&buf[7]))));
    }

    smboffset = 32;

    for (;;) {
	const char *f1, *f2;
	int wct;
	u_int bcc;
	int newsmboffset;

	words = buf + smboffset;
	ND_TCHECK(words[0]);
	wct = words[0];
	data = words + 1 + wct * 2;
	maxwords = min(data, maxbuf);

	if (request) {
	    f1 = fn->descript.req_f1;
	    f2 = fn->descript.req_f2;
	} else {
	    f1 = fn->descript.rep_f1;
	    f2 = fn->descript.rep_f2;
	}

	if (fn->descript.fn)
	    (*fn->descript.fn)(ndo, words, data, buf, maxbuf);
	else {
	    if (wct) {
		if (f1)
		    smb_fdata(ndo, words + 1, f1, words + 1 + wct * 2, unicodestr);
		else {
		    int i;
		    int v;

		    for (i = 0; &words[1 + 2 * i] < maxwords; i++) {
			ND_TCHECK2(words[1 + 2 * i], 2);
			v = EXTRACT_LE_16BITS(words + 1 + 2 * i);
			ND_PRINT((ndo, "smb_vwv[%d]=%d (0x%X)\n", i, v, v));
		    }
		}
	    }

	    ND_TCHECK2(*data, 2);
	    bcc = EXTRACT_LE_16BITS(data);
	    ND_PRINT((ndo, "smb_bcc=%u\n", bcc));
	    if (f2) {
		if (bcc > 0)
		    smb_fdata(ndo, data + 2, f2, data + 2 + bcc, unicodestr);
	    } else {
		if (bcc > 0) {
		    ND_PRINT((ndo, "smb_buf[]=\n"));
		    smb_print_data(ndo, data + 2, min(bcc, PTR_DIFF(maxbuf, data + 2)));
		}
	    }
	}

	if ((fn->flags & FLG_CHAIN) == 0)
	    break;
	if (wct == 0)
	    break;
	ND_TCHECK(words[1]);
	command = words[1];
	if (command == 0xFF)
	    break;
	ND_TCHECK2(words[3], 2);
	newsmboffset = EXTRACT_LE_16BITS(words + 3);

	fn = smbfind(command, smb_fns);

	ND_PRINT((ndo, "\nSMB PACKET: %s (%s) (CHAINED)\n",
	    fn->name, request ? "REQUEST" : "REPLY"));
	if (newsmboffset <= smboffset) {
	    ND_PRINT((ndo, "Bad andX offset: %u <= %u\n", newsmboffset, smboffset));
	    break;
	}
	smboffset = newsmboffset;
    }

    ND_PRINT((ndo, "\n"));
    return;
trunc:
    ND_PRINT((ndo, "%s", tstr));
}
