static int register_xaction_notifier(struct notifier_block *nb)
{
	return atomic_notifier_chain_register(&xaction_notifier_list, nb);
}
