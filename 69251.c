static int tcpmss_tg4_check(const struct xt_tgchk_param *par)
{
	const struct xt_tcpmss_info *info = par->targinfo;
	const struct ipt_entry *e = par->entryinfo;
	const struct xt_entry_match *ematch;

	if (info->mss == XT_TCPMSS_CLAMP_PMTU &&
	    (par->hook_mask & ~((1 << NF_INET_FORWARD) |
			   (1 << NF_INET_LOCAL_OUT) |
			   (1 << NF_INET_POST_ROUTING))) != 0) {
		pr_info("path-MTU clamping only supported in "
			"FORWARD, OUTPUT and POSTROUTING hooks\n");
		return -EINVAL;
	}
	if (par->nft_compat)
		return 0;

	xt_ematch_foreach(ematch, e)
		if (find_syn_match(ematch))
			return 0;
	pr_info("Only works on TCP SYN packets\n");
	return -EINVAL;
}
