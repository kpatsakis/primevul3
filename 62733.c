cpu_init (void)
{
	extern void __cpuinit ia64_mmu_init (void *);
	static unsigned long max_num_phys_stacked = IA64_NUM_PHYS_STACK_REG;
	unsigned long num_phys_stacked;
	pal_vm_info_2_u_t vmi;
	unsigned int max_ctx;
	struct cpuinfo_ia64 *cpu_info;
	void *cpu_data;

	cpu_data = per_cpu_init();
#ifdef CONFIG_SMP
	/*
	 * insert boot cpu into sibling and core mapes
	 * (must be done after per_cpu area is setup)
	 */
	if (smp_processor_id() == 0) {
		cpu_set(0, per_cpu(cpu_sibling_map, 0));
		cpu_set(0, cpu_core_map[0]);
	}
#endif

	/*
	 * We set ar.k3 so that assembly code in MCA handler can compute
	 * physical addresses of per cpu variables with a simple:
	 *   phys = ar.k3 + &per_cpu_var
	 */
	ia64_set_kr(IA64_KR_PER_CPU_DATA,
		    ia64_tpa(cpu_data) - (long) __per_cpu_start);

	get_max_cacheline_size();

	/*
	 * We can't pass "local_cpu_data" to identify_cpu() because we haven't called
	 * ia64_mmu_init() yet.  And we can't call ia64_mmu_init() first because it
	 * depends on the data returned by identify_cpu().  We break the dependency by
	 * accessing cpu_data() through the canonical per-CPU address.
	 */
	cpu_info = cpu_data + ((char *) &__ia64_per_cpu_var(cpu_info) - __per_cpu_start);
	identify_cpu(cpu_info);

#ifdef CONFIG_MCKINLEY
	{
#		define FEATURE_SET 16
		struct ia64_pal_retval iprv;

		if (cpu_info->family == 0x1f) {
			PAL_CALL_PHYS(iprv, PAL_PROC_GET_FEATURES, 0, FEATURE_SET, 0);
			if ((iprv.status == 0) && (iprv.v0 & 0x80) && (iprv.v2 & 0x80))
				PAL_CALL_PHYS(iprv, PAL_PROC_SET_FEATURES,
				              (iprv.v1 | 0x80), FEATURE_SET, 0);
		}
	}
#endif

	/* Clear the stack memory reserved for pt_regs: */
	memset(task_pt_regs(current), 0, sizeof(struct pt_regs));

	ia64_set_kr(IA64_KR_FPU_OWNER, 0);

	/*
	 * Initialize the page-table base register to a global
	 * directory with all zeroes.  This ensure that we can handle
	 * TLB-misses to user address-space even before we created the
	 * first user address-space.  This may happen, e.g., due to
	 * aggressive use of lfetch.fault.
	 */
	ia64_set_kr(IA64_KR_PT_BASE, __pa(ia64_imva(empty_zero_page)));

	/*
	 * Initialize default control register to defer speculative faults except
	 * for those arising from TLB misses, which are not deferred.  The
	 * kernel MUST NOT depend on a particular setting of these bits (in other words,
	 * the kernel must have recovery code for all speculative accesses).  Turn on
	 * dcr.lc as per recommendation by the architecture team.  Most IA-32 apps
	 * shouldn't be affected by this (moral: keep your ia32 locks aligned and you'll
	 * be fine).
	 */
	ia64_setreg(_IA64_REG_CR_DCR,  (  IA64_DCR_DP | IA64_DCR_DK | IA64_DCR_DX | IA64_DCR_DR
					| IA64_DCR_DA | IA64_DCR_DD | IA64_DCR_LC));
	atomic_inc(&init_mm.mm_count);
	current->active_mm = &init_mm;
	if (current->mm)
		BUG();

	ia64_mmu_init(ia64_imva(cpu_data));
	ia64_mca_cpu_init(ia64_imva(cpu_data));

#ifdef CONFIG_IA32_SUPPORT
	ia32_cpu_init();
#endif

	/* Clear ITC to eliminate sched_clock() overflows in human time.  */
	ia64_set_itc(0);

	/* disable all local interrupt sources: */
	ia64_set_itv(1 << 16);
	ia64_set_lrr0(1 << 16);
	ia64_set_lrr1(1 << 16);
	ia64_setreg(_IA64_REG_CR_PMV, 1 << 16);
	ia64_setreg(_IA64_REG_CR_CMCV, 1 << 16);

	/* clear TPR & XTP to enable all interrupt classes: */
	ia64_setreg(_IA64_REG_CR_TPR, 0);

	/* Clear any pending interrupts left by SAL/EFI */
	while (ia64_get_ivr() != IA64_SPURIOUS_INT_VECTOR)
		ia64_eoi();

#ifdef CONFIG_SMP
	normal_xtp();
#endif

	/* set ia64_ctx.max_rid to the maximum RID that is supported by all CPUs: */
	if (ia64_pal_vm_summary(NULL, &vmi) == 0) {
		max_ctx = (1U << (vmi.pal_vm_info_2_s.rid_size - 3)) - 1;
		setup_ptcg_sem(vmi.pal_vm_info_2_s.max_purges, NPTCG_FROM_PAL);
	} else {
		printk(KERN_WARNING "cpu_init: PAL VM summary failed, assuming 18 RID bits\n");
		max_ctx = (1U << 15) - 1;	/* use architected minimum */
	}
	while (max_ctx < ia64_ctx.max_ctx) {
		unsigned int old = ia64_ctx.max_ctx;
		if (cmpxchg(&ia64_ctx.max_ctx, old, max_ctx) == old)
			break;
	}

	if (ia64_pal_rse_info(&num_phys_stacked, NULL) != 0) {
		printk(KERN_WARNING "cpu_init: PAL RSE info failed; assuming 96 physical "
		       "stacked regs\n");
		num_phys_stacked = 96;
	}
	/* size of physical stacked register partition plus 8 bytes: */
	if (num_phys_stacked > max_num_phys_stacked) {
		ia64_patch_phys_stack_reg(num_phys_stacked*8 + 8);
		max_num_phys_stacked = num_phys_stacked;
	}
	platform_cpu_init();
	pm_idle = default_idle;
}
