decode_rt_routing_info(netdissect_options *ndo,
                       const u_char *pptr, char *buf, u_int buflen)
{
	uint8_t route_target[8];
	u_int plen;
	char asbuf[sizeof(astostr)]; /* bgp_vpn_rd_print() overwrites astostr */

	/* NLRI "prefix length" from RFC 2858 Section 4. */
	ND_TCHECK(pptr[0]);
	plen = pptr[0];   /* get prefix length */

	/* NLRI "prefix" (ibid), valid lengths are { 0, 32, 33, ..., 96 } bits.
	 * RFC 4684 Section 4 defines the layout of "origin AS" and "route
	 * target" fields inside the "prefix" depending on its length.
	 */
	if (0 == plen) {
		/* Without "origin AS", without "route target". */
		snprintf(buf, buflen, "default route target");
		return 1;
	}

	if (32 > plen)
		return -1;

	/* With at least "origin AS", possibly with "route target". */
	ND_TCHECK_32BITS(pptr + 1);
	as_printf(ndo, asbuf, sizeof(asbuf), EXTRACT_32BITS(pptr + 1));

        plen-=32; /* adjust prefix length */

	if (64 < plen)
		return -1;

	/* From now on (plen + 7) / 8 evaluates to { 0, 1, 2, ..., 8 }
	 * and gives the number of octets in the variable-length "route
	 * target" field inside this NLRI "prefix". Look for it.
	 */
	memset(&route_target, 0, sizeof(route_target));
	ND_TCHECK2(pptr[5], (plen + 7) / 8);
	memcpy(&route_target, &pptr[5], (plen + 7) / 8);
	/* Which specification says to do this? */
	if (plen % 8) {
		((u_char *)&route_target)[(plen + 7) / 8 - 1] &=
			((0xff00 >> (plen % 8)) & 0xff);
	}
	snprintf(buf, buflen, "origin AS: %s, route target %s",
	    asbuf,
	    bgp_vpn_rd_print(ndo, (u_char *)&route_target));

	return 5 + (plen + 7) / 8;

trunc:
	return -2;
}
