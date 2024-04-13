cmu_print(netdissect_options *ndo,
	  register const u_char *bp)
{
	register const struct cmu_vend *cmu;

#define PRINTCMUADDR(m, s) { ND_TCHECK(cmu->m); \
    if (cmu->m.s_addr != 0) \
	ND_PRINT((ndo, " %s:%s", s, ipaddr_string(ndo, &cmu->m.s_addr))); }

	ND_PRINT((ndo, " vend-cmu"));
	cmu = (const struct cmu_vend *)bp;

	/* Only print if there are unknown bits */
	ND_TCHECK(cmu->v_flags);
	if ((cmu->v_flags & ~(VF_SMASK)) != 0)
		ND_PRINT((ndo, " F:0x%x", cmu->v_flags));
	PRINTCMUADDR(v_dgate, "DG");
	PRINTCMUADDR(v_smask, cmu->v_flags & VF_SMASK ? "SM" : "SM*");
	PRINTCMUADDR(v_dns1, "NS1");
	PRINTCMUADDR(v_dns2, "NS2");
	PRINTCMUADDR(v_ins1, "IEN1");
	PRINTCMUADDR(v_ins2, "IEN2");
	PRINTCMUADDR(v_ts1, "TS1");
	PRINTCMUADDR(v_ts2, "TS2");
	return;

trunc:
	ND_PRINT((ndo, "%s", tstr));
#undef PRINTCMUADDR
}
