print_trans2(netdissect_options *ndo,
             const u_char *words, const u_char *dat, const u_char *buf, const u_char *maxbuf)
{
    u_int bcc;
    static const struct smbfnsint *fn = &trans2_fns[0];
    const u_char *data, *param;
    const u_char *w = words + 1;
    const char *f1 = NULL, *f2 = NULL;
    int pcnt, dcnt;

    ND_TCHECK(words[0]);
    if (request) {
	ND_TCHECK2(w[14 * 2], 2);
	pcnt = EXTRACT_LE_16BITS(w + 9 * 2);
	param = buf + EXTRACT_LE_16BITS(w + 10 * 2);
	dcnt = EXTRACT_LE_16BITS(w + 11 * 2);
	data = buf + EXTRACT_LE_16BITS(w + 12 * 2);
	fn = smbfindint(EXTRACT_LE_16BITS(w + 14 * 2), trans2_fns);
    } else {
	if (words[0] == 0) {
	    ND_PRINT((ndo, "%s\n", fn->name));
	    ND_PRINT((ndo, "Trans2Interim\n"));
	    return;
	}
	ND_TCHECK2(w[7 * 2], 2);
	pcnt = EXTRACT_LE_16BITS(w + 3 * 2);
	param = buf + EXTRACT_LE_16BITS(w + 4 * 2);
	dcnt = EXTRACT_LE_16BITS(w + 6 * 2);
	data = buf + EXTRACT_LE_16BITS(w + 7 * 2);
    }

    ND_PRINT((ndo, "%s param_length=%d data_length=%d\n", fn->name, pcnt, dcnt));

    if (request) {
	if (words[0] == 8) {
	    smb_fdata(ndo, words + 1,
		"Trans2Secondary\nTotParam=[d]\nTotData=[d]\nParamCnt=[d]\nParamOff=[d]\nParamDisp=[d]\nDataCnt=[d]\nDataOff=[d]\nDataDisp=[d]\nHandle=[d]\n",
		maxbuf, unicodestr);
	    return;
	} else {
	    smb_fdata(ndo, words + 1,
		"TotParam=[d]\nTotData=[d]\nMaxParam=[d]\nMaxData=[d]\nMaxSetup=[b][P1]\nFlags=[w]\nTimeOut=[D]\nRes1=[w]\nParamCnt=[d]\nParamOff=[d]\nDataCnt=[d]\nDataOff=[d]\nSetupCnt=[b][P1]\n",
		words + 1 + 14 * 2, unicodestr);
	}
	f1 = fn->descript.req_f1;
	f2 = fn->descript.req_f2;
    } else {
	smb_fdata(ndo, words + 1,
	    "TotParam=[d]\nTotData=[d]\nRes1=[w]\nParamCnt=[d]\nParamOff=[d]\nParamDisp[d]\nDataCnt=[d]\nDataOff=[d]\nDataDisp=[d]\nSetupCnt=[b][P1]\n",
	    words + 1 + 10 * 2, unicodestr);
	f1 = fn->descript.rep_f1;
	f2 = fn->descript.rep_f2;
    }

    ND_TCHECK2(*dat, 2);
    bcc = EXTRACT_LE_16BITS(dat);
    ND_PRINT((ndo, "smb_bcc=%u\n", bcc));
    if (fn->descript.fn)
	(*fn->descript.fn)(ndo, param, data, pcnt, dcnt);
    else {
	smb_fdata(ndo, param, f1 ? f1 : "Parameters=\n", param + pcnt, unicodestr);
	smb_fdata(ndo, data, f2 ? f2 : "Data=\n", data + dcnt, unicodestr);
    }
    return;
trunc:
    ND_PRINT((ndo, "%s", tstr));
}
