int unregister_die_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_unregister(&powerpc_die_chain, nb);
}
