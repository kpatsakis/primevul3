evutil_found_ifaddr(const struct sockaddr *sa)
{
	const char ZEROES[] = "\x00\x00\x00\x00\x00\x00\x00\x00"
	    "\x00\x00\x00\x00\x00\x00\x00\x00";

	if (sa->sa_family == AF_INET) {
		const struct sockaddr_in *sin = (struct sockaddr_in *)sa;
		ev_uint32_t addr = ntohl(sin->sin_addr.s_addr);
		if (addr == 0 ||
		    EVUTIL_V4ADDR_IS_LOCALHOST(addr) ||
		    EVUTIL_V4ADDR_IS_CLASSD(addr)) {
			/* Not actually a usable external address. */
		} else {
			event_debug(("Detected an IPv4 interface"));
			had_ipv4_address = 1;
		}
	} else if (sa->sa_family == AF_INET6) {
		const struct sockaddr_in6 *sin6 = (struct sockaddr_in6 *)sa;
		const unsigned char *addr =
		    (unsigned char*)sin6->sin6_addr.s6_addr;
		if (!memcmp(addr, ZEROES, 8) ||
		    ((addr[0] & 0xfe) == 0xfc) ||
		    (addr[0] == 0xfe && (addr[1] & 0xc0) == 0x80) ||
		    (addr[0] == 0xfe && (addr[1] & 0xc0) == 0xc0) ||
		    (addr[0] == 0xff)) {
			/* This is a reserved, ipv4compat, ipv4map, loopback,
			 * link-local, multicast, or unspecified address. */
		} else {
			event_debug(("Detected an IPv6 interface"));
			had_ipv6_address = 1;
		}
	}
}
