ia64_patch_gate (void)
{
#	define START(name)	((unsigned long) __start_gate_##name##_patchlist)
#	define END(name)	((unsigned long)__end_gate_##name##_patchlist)

	patch_fsyscall_table(START(fsyscall), END(fsyscall));
	patch_brl_fsys_bubble_down(START(brl_fsys_bubble_down), END(brl_fsys_bubble_down));
	ia64_patch_vtop(START(vtop), END(vtop));
	ia64_patch_mckinley_e9(START(mckinley_e9), END(mckinley_e9));
}
