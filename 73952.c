print_l1_routes(netdissect_options *ndo,
                const char *rp, u_int len)
{
	int count;
	int id;
	int info;

	/* The last short is a checksum */
	while (len > (3 * sizeof(short))) {
	    ND_TCHECK2(*rp, 3 * sizeof(short));
	    count = EXTRACT_LE_16BITS(rp);
	    if (count > 1024)
		return (1);	/* seems to be bogus from here on */
	    rp += sizeof(short);
	    len -= sizeof(short);
	    id = EXTRACT_LE_16BITS(rp);
	    rp += sizeof(short);
	    len -= sizeof(short);
	    info = EXTRACT_LE_16BITS(rp);
	    rp += sizeof(short);
	    len -= sizeof(short);
	    ND_PRINT((ndo, "{ids %d-%d cost %d hops %d} ", id, id + count,
			    RI_COST(info), RI_HOPS(info)));
	}
	return (1);

trunc:
	return (0);
}
