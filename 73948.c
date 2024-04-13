dnname_string(netdissect_options *ndo, u_short dnaddr)
{
#ifdef HAVE_DNET_HTOA
	struct dn_naddr dna;
	char *dnname;

	dna.a_len = sizeof(short);
	memcpy((char *)dna.a_addr, (char *)&dnaddr, sizeof(short));
	dnname = dnet_htoa(&dna);
	if(dnname != NULL)
		return (strdup(dnname));
	else
		return(dnnum_string(ndo, dnaddr));
#else
	return(dnnum_string(ndo, dnaddr));	/* punt */
#endif
}
