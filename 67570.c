do_simd_coprocessor_error(struct pt_regs *regs, long error_code)
{
	RCU_LOCKDEP_WARN(!rcu_is_watching(), "entry code didn't wake RCU");
	math_error(regs, error_code, X86_TRAP_XF);
}
