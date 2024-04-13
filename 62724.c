void ia64_patch_phys_stack_reg(unsigned long val)
{
	s32 * offp = (s32 *) __start___phys_stack_reg_patchlist;
	s32 * end = (s32 *) __end___phys_stack_reg_patchlist;
	u64 ip, mask, imm;

	/* see instruction format A4: adds r1 = imm13, r3 */
	mask = (0x3fUL << 27) | (0x7f << 13);
	imm = (((val >> 7) & 0x3f) << 27) | (val & 0x7f) << 13;

	while (offp < end) {
		ip = (u64) offp + *offp;
		ia64_patch(ip, mask, imm);
		ia64_fc(ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}
