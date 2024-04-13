print_ipc(netdissect_options *ndo,
          const u_char *param, int paramlen, const u_char *data, int datalen)
{
    if (paramlen)
	smb_fdata(ndo, param, "Command=[w]\nStr1=[S]\nStr2=[S]\n", param + paramlen,
	    unicodestr);
    if (datalen)
	smb_fdata(ndo, data, "IPC ", data + datalen, unicodestr);
}
