void radeon_atom_set_engine_clock(struct radeon_device *rdev,
				  uint32_t eng_clock)
{
	SET_ENGINE_CLOCK_PS_ALLOCATION args;
	int index = GetIndexIntoMasterTable(COMMAND, SetEngineClock);

	args.ulTargetEngineClock = eng_clock;	/* 10 khz */

	atom_execute_table(rdev->mode_info.atom_context, index, (uint32_t *)&args);
}
