void radeon_atom_output_lock(struct drm_encoder *encoder, bool lock)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	uint32_t bios_6_scratch;

	if (rdev->family >= CHIP_R600)
		bios_6_scratch = RREG32(R600_BIOS_6_SCRATCH);
	else
		bios_6_scratch = RREG32(RADEON_BIOS_6_SCRATCH);

	if (lock)
		bios_6_scratch |= ATOM_S6_CRITICAL_STATE;
	else
		bios_6_scratch &= ~ATOM_S6_CRITICAL_STATE;

	if (rdev->family >= CHIP_R600)
		WREG32(R600_BIOS_6_SCRATCH, bios_6_scratch);
	else
		WREG32(RADEON_BIOS_6_SCRATCH, bios_6_scratch);
}
