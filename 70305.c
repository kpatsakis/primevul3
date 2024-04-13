daemon_unpackapplyfilter(SOCKET sockctrl, struct session *session, uint32 *plenp, char *errmsgbuf)
{
	int status;
	struct rpcap_filter filter;
	struct rpcap_filterbpf_insn insn;
	struct bpf_insn *bf_insn;
	struct bpf_program bf_prog;
	unsigned int i;

	status = rpcapd_recv(sockctrl, (char *) &filter,
	    sizeof(struct rpcap_filter), plenp, errmsgbuf);
	if (status == -1)
	{
		return -1;
	}
	if (status == -2)
	{
		return -2;
	}

	bf_prog.bf_len = ntohl(filter.nitems);

	if (ntohs(filter.filtertype) != RPCAP_UPDATEFILTER_BPF)
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "Only BPF/NPF filters are currently supported");
		return -2;
	}

	bf_insn = (struct bpf_insn *) malloc (sizeof(struct bpf_insn) * bf_prog.bf_len);
	if (bf_insn == NULL)
	{
		pcap_fmt_errmsg_for_errno(errmsgbuf, PCAP_ERRBUF_SIZE,
		    errno, "malloc() failed");
		return -2;
	}

	bf_prog.bf_insns = bf_insn;

	for (i = 0; i < bf_prog.bf_len; i++)
	{
		status = rpcapd_recv(sockctrl, (char *) &insn,
		    sizeof(struct rpcap_filterbpf_insn), plenp, errmsgbuf);
		if (status == -1)
		{
			return -1;
		}
		if (status == -2)
		{
			return -2;
		}

		bf_insn->code = ntohs(insn.code);
		bf_insn->jf = insn.jf;
		bf_insn->jt = insn.jt;
		bf_insn->k = ntohl(insn.k);

		bf_insn++;
	}

	if (bpf_validate(bf_prog.bf_insns, bf_prog.bf_len) == 0)
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "The filter contains bogus instructions");
		return -2;
	}

	if (pcap_setfilter(session->fp, &bf_prog))
	{
		pcap_snprintf(errmsgbuf, PCAP_ERRBUF_SIZE, "RPCAP error: %s", pcap_geterr(session->fp));
		return -2;
	}

	return 0;
}
