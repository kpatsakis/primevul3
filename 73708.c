void radeon_atom_set_memory_clock(struct radeon_device *rdev,
				  uint32_t mem_clock)
{
	SET_MEMORY_CLOCK_PS_ALLOCATION args;
	int index = GetIndexIntoMasterTable(COMMAND, SetMemoryClock);

	if (rdev->flags & RADEON_IS_IGP)
		return;

	args.ulTargetMemoryClock = mem_clock;	/* 10 khz */

	atom_execute_table(rdev->mode_info.atom_context, index, (uint32_t *)&args);
}
