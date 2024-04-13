bgp_notification_print(netdissect_options *ndo,
                       const u_char *dat, int length)
{
	struct bgp_notification bgpn;
	const u_char *tptr;

	ND_TCHECK2(dat[0], BGP_NOTIFICATION_SIZE);
	memcpy(&bgpn, dat, BGP_NOTIFICATION_SIZE);

        /* some little sanity checking */
        if (length<BGP_NOTIFICATION_SIZE)
            return;

	ND_PRINT((ndo, ", %s (%u)",
	       tok2str(bgp_notify_major_values, "Unknown Error",
			  bgpn.bgpn_major),
	       bgpn.bgpn_major));

        switch (bgpn.bgpn_major) {

        case BGP_NOTIFY_MAJOR_MSG:
            ND_PRINT((ndo, ", subcode %s (%u)",
		   tok2str(bgp_notify_minor_msg_values, "Unknown",
			      bgpn.bgpn_minor),
		   bgpn.bgpn_minor));
            break;
        case BGP_NOTIFY_MAJOR_OPEN:
            ND_PRINT((ndo, ", subcode %s (%u)",
		   tok2str(bgp_notify_minor_open_values, "Unknown",
			      bgpn.bgpn_minor),
		   bgpn.bgpn_minor));
            break;
        case BGP_NOTIFY_MAJOR_UPDATE:
            ND_PRINT((ndo, ", subcode %s (%u)",
		   tok2str(bgp_notify_minor_update_values, "Unknown",
			      bgpn.bgpn_minor),
		   bgpn.bgpn_minor));
            break;
        case BGP_NOTIFY_MAJOR_FSM:
            ND_PRINT((ndo, " subcode %s (%u)",
		   tok2str(bgp_notify_minor_fsm_values, "Unknown",
			      bgpn.bgpn_minor),
		   bgpn.bgpn_minor));
            break;
        case BGP_NOTIFY_MAJOR_CAP:
            ND_PRINT((ndo, " subcode %s (%u)",
		   tok2str(bgp_notify_minor_cap_values, "Unknown",
			      bgpn.bgpn_minor),
		   bgpn.bgpn_minor));
            break;
        case BGP_NOTIFY_MAJOR_CEASE:
            ND_PRINT((ndo, ", subcode %s (%u)",
		   tok2str(bgp_notify_minor_cease_values, "Unknown",
			      bgpn.bgpn_minor),
		   bgpn.bgpn_minor));

	    /* draft-ietf-idr-cease-subcode-02 mentions optionally 7 bytes
             * for the maxprefix subtype, which may contain AFI, SAFI and MAXPREFIXES
             */
	    if(bgpn.bgpn_minor == BGP_NOTIFY_MINOR_CEASE_MAXPRFX && length >= BGP_NOTIFICATION_SIZE + 7) {
		tptr = dat + BGP_NOTIFICATION_SIZE;
		ND_TCHECK2(*tptr, 7);
		ND_PRINT((ndo, ", AFI %s (%u), SAFI %s (%u), Max Prefixes: %u",
		       tok2str(af_values, "Unknown",
				  EXTRACT_16BITS(tptr)),
		       EXTRACT_16BITS(tptr),
		       tok2str(bgp_safi_values, "Unknown", *(tptr+2)),
		       *(tptr+2),
		       EXTRACT_32BITS(tptr+3)));
	    }
            break;
        default:
            break;
        }

	return;
trunc:
	ND_PRINT((ndo, "[|BGP]"));
}
