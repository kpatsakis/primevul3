ofproto_bump_tables_version(struct ofproto *ofproto)
{
    ++ofproto->tables_version;
    ofproto->ofproto_class->set_tables_version(ofproto,
                                               ofproto->tables_version);
}
