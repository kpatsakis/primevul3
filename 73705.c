uint32_t radeon_atom_get_memory_clock(struct radeon_device *rdev)
{
	GET_MEMORY_CLOCK_PS_ALLOCATION args;
	int index = GetIndexIntoMasterTable(COMMAND, GetMemoryClock);

	atom_execute_table(rdev->mode_info.atom_context, index, (uint32_t *)&args);
	return args.ulReturnMemoryClock;
}
