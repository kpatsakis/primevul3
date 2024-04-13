ofproto_flush_flows(struct ofproto *ofproto)
{
    COVERAGE_INC(ofproto_flush);
    ofproto_flush__(ofproto);
    connmgr_flushed(ofproto->connmgr);
}
