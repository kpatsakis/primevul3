print_i_info(netdissect_options *ndo,
             int info)
{
	int ntype = info & II_TYPEMASK;
	switch (ntype) {
	case 0: ND_PRINT((ndo, "reserved-ntype? ")); break;
	case II_L2ROUT: ND_PRINT((ndo, "l2rout ")); break;
	case II_L1ROUT: ND_PRINT((ndo, "l1rout ")); break;
	case II_ENDNODE: ND_PRINT((ndo, "endnode ")); break;
	}
	if (info & II_VERIF)
	    ND_PRINT((ndo, "verif "));
	if (info & II_NOMCAST)
	    ND_PRINT((ndo, "nomcast "));
	if (info & II_BLOCK)
	    ND_PRINT((ndo, "blo "));
}
