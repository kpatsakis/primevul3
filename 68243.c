DetectRunPostGetFirstRuleGroup(const Packet *p, Flow *pflow, const SigGroupHead *sgh)
{
    if ((p->flowflags & FLOW_PKT_TOSERVER) && !(pflow->flags & FLOW_SGH_TOSERVER)) {
        /* first time we see this toserver sgh, store it */
        pflow->sgh_toserver = sgh;
        pflow->flags |= FLOW_SGH_TOSERVER;

        if (p->proto == IPPROTO_TCP && (sgh == NULL || !(sgh->flags & SIG_GROUP_HEAD_HAVERAWSTREAM))) {
            if (pflow->protoctx != NULL) {
                TcpSession *ssn = pflow->protoctx;
                SCLogDebug("STREAMTCP_STREAM_FLAG_DISABLE_RAW ssn.client");
                ssn->client.flags |= STREAMTCP_STREAM_FLAG_DISABLE_RAW;
            }
        }

        DetectPostInspectFileFlagsUpdate(pflow,
                pflow->sgh_toserver, STREAM_TOSERVER);

    } else if ((p->flowflags & FLOW_PKT_TOCLIENT) && !(pflow->flags & FLOW_SGH_TOCLIENT)) {
        pflow->sgh_toclient = sgh;
        pflow->flags |= FLOW_SGH_TOCLIENT;

        if (p->proto == IPPROTO_TCP && (sgh == NULL || !(sgh->flags & SIG_GROUP_HEAD_HAVERAWSTREAM))) {
            if (pflow->protoctx != NULL) {
                TcpSession *ssn = pflow->protoctx;
                SCLogDebug("STREAMTCP_STREAM_FLAG_DISABLE_RAW ssn.server");
                ssn->server.flags |= STREAMTCP_STREAM_FLAG_DISABLE_RAW;
            }
        }

        DetectPostInspectFileFlagsUpdate(pflow,
                pflow->sgh_toclient, STREAM_TOCLIENT);
    }
}
