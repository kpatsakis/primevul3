static enum ssb_mitigation __init __ssb_select_mitigation(void)
{
	enum ssb_mitigation mode = SPEC_STORE_BYPASS_NONE;
	enum ssb_mitigation_cmd cmd;

	if (!boot_cpu_has(X86_FEATURE_SSBD))
		return mode;

	cmd = ssb_parse_cmdline();
	if (!boot_cpu_has_bug(X86_BUG_SPEC_STORE_BYPASS) &&
	    (cmd == SPEC_STORE_BYPASS_CMD_NONE ||
	     cmd == SPEC_STORE_BYPASS_CMD_AUTO))
		return mode;

	switch (cmd) {
	case SPEC_STORE_BYPASS_CMD_AUTO:
	case SPEC_STORE_BYPASS_CMD_SECCOMP:
		/*
		 * Choose prctl+seccomp as the default mode if seccomp is
		 * enabled.
		 */
		if (IS_ENABLED(CONFIG_SECCOMP))
			mode = SPEC_STORE_BYPASS_SECCOMP;
		else
			mode = SPEC_STORE_BYPASS_PRCTL;
		break;
	case SPEC_STORE_BYPASS_CMD_ON:
		mode = SPEC_STORE_BYPASS_DISABLE;
		break;
	case SPEC_STORE_BYPASS_CMD_PRCTL:
		mode = SPEC_STORE_BYPASS_PRCTL;
		break;
	case SPEC_STORE_BYPASS_CMD_NONE:
		break;
	}

	/*
	 * We have three CPU feature flags that are in play here:
	 *  - X86_BUG_SPEC_STORE_BYPASS - CPU is susceptible.
	 *  - X86_FEATURE_SSBD - CPU is able to turn off speculative store bypass
	 *  - X86_FEATURE_SPEC_STORE_BYPASS_DISABLE - engage the mitigation
	 */
	if (mode == SPEC_STORE_BYPASS_DISABLE) {
		setup_force_cpu_cap(X86_FEATURE_SPEC_STORE_BYPASS_DISABLE);
		/*
		 * Intel uses the SPEC CTRL MSR Bit(2) for this, while AMD may
		 * use a completely different MSR and bit dependent on family.
		 */
		if (!static_cpu_has(X86_FEATURE_SPEC_CTRL_SSBD) &&
		    !static_cpu_has(X86_FEATURE_AMD_SSBD)) {
			x86_amd_ssb_disable();
		} else {
			x86_spec_ctrl_base |= SPEC_CTRL_SSBD;
			x86_spec_ctrl_mask |= SPEC_CTRL_SSBD;
			wrmsrl(MSR_IA32_SPEC_CTRL, x86_spec_ctrl_base);
		}
	}

	return mode;
}
