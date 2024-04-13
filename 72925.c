ipx_netbios_print(netdissect_options *ndo,
                  const u_char *data, u_int length)
{
    /*
     * this is a hack till I work out how to parse the rest of the
     * NetBIOS-over-IPX stuff
     */
    int i;
    const u_char *maxbuf;

    maxbuf = data + length;
    /* Don't go past the end of the captured data in the packet. */
    if (maxbuf > ndo->ndo_snapend)
	maxbuf = ndo->ndo_snapend;
    startbuf = data;
    for (i = 0; i < 128; i++) {
	if (&data[i + 4] > maxbuf)
	    break;
	if (memcmp(&data[i], "\377SMB", 4) == 0) {
	    smb_fdata(ndo, data, "\n>>> IPX transport ", &data[i], 0);
	    print_smb(ndo, &data[i], maxbuf);
	    ND_PRINT((ndo, "\n"));
	    break;
	}
    }
    if (i == 128)
	smb_fdata(ndo, data, "\n>>> Unknown IPX ", maxbuf, 0);
}
