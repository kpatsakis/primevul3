static int __init nf_nat_snmp_basic_init(void)
{
	BUG_ON(nf_nat_snmp_hook != NULL);
	RCU_INIT_POINTER(nf_nat_snmp_hook, help);

	return nf_conntrack_helper_register(&snmp_trap_helper);
}
