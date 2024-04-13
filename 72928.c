nbt_udp138_print(netdissect_options *ndo,
                 const u_char *data, int length)
{
    const u_char *maxbuf = data + length;

    if (maxbuf > ndo->ndo_snapend)
	maxbuf = ndo->ndo_snapend;
    if (maxbuf <= data)
	return;
    startbuf = data;

    if (ndo->ndo_vflag < 2) {
	ND_PRINT((ndo, "NBT UDP PACKET(138)"));
	return;
    }

    data = smb_fdata(ndo, data,
	"\n>>> NBT UDP PACKET(138) Res=[rw] ID=[rw] IP=[b.b.b.b] Port=[rd] Length=[rd] Res2=[rw]\nSourceName=[n1]\nDestName=[n1]\n#",
	maxbuf, 0);

    if (data != NULL) {
	/* If there isn't enough data for "\377SMB", don't check for it. */
	if (&data[3] >= maxbuf)
	    goto out;

	if (memcmp(data, "\377SMB",4) == 0)
	    print_smb(ndo, data, maxbuf);
    }
out:
    ND_PRINT((ndo, "\n"));
}
