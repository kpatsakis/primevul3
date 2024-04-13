static void __exit nf_nat_snmp_basic_fini(void)
{
	RCU_INIT_POINTER(nf_nat_snmp_hook, NULL);
	synchronize_rcu();
	nf_conntrack_helper_unregister(&snmp_trap_helper);
}
