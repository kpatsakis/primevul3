patch_fsyscall_table (unsigned long start, unsigned long end)
{
	extern unsigned long fsyscall_table[NR_syscalls];
	s32 *offp = (s32 *) start;
	u64 ip;

	while (offp < (s32 *) end) {
		ip = (u64) ia64_imva((char *) offp + *offp);
		ia64_patch_imm64(ip, (u64) fsyscall_table);
		ia64_fc((void *) ip);
		++offp;
	}
	ia64_sync_i();
	ia64_srlz_i();
}
