print_sesssetup(netdissect_options *ndo,
                const u_char *words, const u_char *data, const u_char *buf _U_, const u_char *maxbuf)
{
    u_int wct, bcc;
    const char *f1 = NULL, *f2 = NULL;

    ND_TCHECK(words[0]);
    wct = words[0];
    if (request) {
	if (wct == 10)
	    f1 = "Com2=[w]\nOff2=[d]\nBufSize=[d]\nMpxMax=[d]\nVcNum=[d]\nSessionKey=[W]\nPassLen=[d]\nCryptLen=[d]\nCryptOff=[d]\nPass&Name=\n";
	else
	    f1 = "Com2=[B]\nRes1=[B]\nOff2=[d]\nMaxBuffer=[d]\nMaxMpx=[d]\nVcNumber=[d]\nSessionKey=[W]\nCaseInsensitivePasswordLength=[d]\nCaseSensitivePasswordLength=[d]\nRes=[W]\nCapabilities=[W]\nPass1&Pass2&Account&Domain&OS&LanMan=\n";
    } else {
	if (wct == 3) {
	    f1 = "Com2=[w]\nOff2=[d]\nAction=[w]\n";
	} else if (wct == 13) {
	    f1 = "Com2=[B]\nRes=[B]\nOff2=[d]\nAction=[w]\n";
	    f2 = "NativeOS=[S]\nNativeLanMan=[S]\nPrimaryDomain=[S]\n";
	}
    }

    if (f1)
	smb_fdata(ndo, words + 1, f1, min(words + 1 + wct * 2, maxbuf),
	    unicodestr);
    else
	smb_print_data(ndo, words + 1, min(wct * 2, PTR_DIFF(maxbuf, words + 1)));

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
