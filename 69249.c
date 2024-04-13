static inline bool find_syn_match(const struct xt_entry_match *m)
{
	const struct xt_tcp *tcpinfo = (const struct xt_tcp *)m->data;

	if (strcmp(m->u.kernel.match->name, "tcp") == 0 &&
	    tcpinfo->flg_cmp & TCPHDR_SYN &&
	    !(tcpinfo->invflags & XT_TCP_INV_FLAGS))
		return true;

	return false;
}
