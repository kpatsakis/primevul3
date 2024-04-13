print_reason(netdissect_options *ndo,
             register int reason)
{
	ND_PRINT((ndo, "%s ", tok2str(reason2str, "reason-%d", reason)));
}
