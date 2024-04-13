patch_brl_fsys_bubble_down (unsigned long start, unsigned long end)
{
	extern char fsys_bubble_down[];
	s32 *offp = (s32 *) start;
	u64 ip;

	while (offp < (s32 *) end) {
		ip = (u64) offp + *offp;
		ia64_patch_imm60((u64) ia64_imva((void *) ip),
				 (u64) (fsys_bubble_down - (ip & -16)) / 16);
		ia64_fc((void *) ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}
