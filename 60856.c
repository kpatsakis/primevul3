static void grow_halt_poll_ns(struct kvm_vcpu *vcpu)
{
	unsigned int old, val, grow;

	old = val = vcpu->halt_poll_ns;
	grow = READ_ONCE(halt_poll_ns_grow);
	/* 10us base */
	if (val == 0 && grow)
		val = 10000;
	else
		val *= grow;

	if (val > halt_poll_ns)
		val = halt_poll_ns;

	vcpu->halt_poll_ns = val;
	trace_kvm_halt_poll_ns_grow(vcpu->vcpu_id, val, old);
}
