void radeon_save_bios_scratch_regs(struct radeon_device *rdev)
{
	uint32_t scratch_reg;
	int i;

	if (rdev->family >= CHIP_R600)
		scratch_reg = R600_BIOS_0_SCRATCH;
	else
		scratch_reg = RADEON_BIOS_0_SCRATCH;

	for (i = 0; i < RADEON_BIOS_NUM_SCRATCH; i++)
		rdev->bios_scratch[i] = RREG32(scratch_reg + (i * 4));
}
