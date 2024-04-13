trans2_findfirst(netdissect_options *ndo,
                 const u_char *param, const u_char *data, int pcnt, int dcnt)
{
    const char *fmt;

    if (request)
	fmt = "Attribute=[A]\nSearchCount=[d]\nFlags=[w]\nLevel=[dP4]\nFile=[S]\n";
    else
	fmt = "Handle=[w]\nCount=[d]\nEOS=[w]\nEoffset=[d]\nLastNameOfs=[w]\n";

    smb_fdata(ndo, param, fmt, param + pcnt, unicodestr);
    if (dcnt) {
	ND_PRINT((ndo, "data:\n"));
	smb_print_data(ndo, data, dcnt);
    }
}
