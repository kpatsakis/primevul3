ldp_print(netdissect_options *ndo,
          register const u_char *pptr, register u_int len)
{
    u_int processed;
    while (len > (sizeof(struct ldp_common_header) + sizeof(struct ldp_msg_header))) {
        processed = ldp_pdu_print(ndo, pptr);
        if (processed == 0)
            return;
        if (len < processed) {
            ND_PRINT((ndo, " [remaining length %u < %u]", len, processed));
            ND_PRINT((ndo, "%s", istr));
            break;

        }
        len -= processed;
        pptr += processed;
    }
}
