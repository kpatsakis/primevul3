int register_die_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&powerpc_die_chain, nb);
}
