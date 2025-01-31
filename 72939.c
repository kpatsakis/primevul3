trans2_qfsinfo(netdissect_options *ndo,
               const u_char *param, const u_char *data, int pcnt, int dcnt)
{
    static int level = 0;
    const char *fmt="";

    if (request) {
	ND_TCHECK2(*param, 2);
	level = EXTRACT_LE_16BITS(param);
	fmt = "InfoLevel=[d]\n";
	smb_fdata(ndo, param, fmt, param + pcnt, unicodestr);
    } else {
	switch (level) {
	case 1:
	    fmt = "idFileSystem=[W]\nSectorUnit=[D]\nUnit=[D]\nAvail=[D]\nSectorSize=[d]\n";
	    break;
	case 2:
	    fmt = "CreationTime=[T2]VolNameLength=[lb]\nVolumeLabel=[c]\n";
	    break;
	case 0x105:
	    fmt = "Capabilities=[W]\nMaxFileLen=[D]\nVolNameLen=[lD]\nVolume=[C]\n";
	    break;
	default:
	    fmt = "UnknownLevel\n";
	    break;
	}
	smb_fdata(ndo, data, fmt, data + dcnt, unicodestr);
    }
    if (dcnt) {
	ND_PRINT((ndo, "data:\n"));
	smb_print_data(ndo, data, dcnt);
    }
    return;
trunc:
    ND_PRINT((ndo, "%s", tstr));
}
