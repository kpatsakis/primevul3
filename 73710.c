radeon_atombios_encoder_crtc_scratch_regs(struct drm_encoder *encoder, int crtc)
{
	struct drm_device *dev = encoder->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	uint32_t bios_3_scratch;

	if (rdev->family >= CHIP_R600)
		bios_3_scratch = RREG32(R600_BIOS_3_SCRATCH);
	else
		bios_3_scratch = RREG32(RADEON_BIOS_3_SCRATCH);

	if (radeon_encoder->devices & ATOM_DEVICE_TV1_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_TV1_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 18);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_CV_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_CV_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 24);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_CRT1_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_CRT1_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 16);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_CRT2_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_CRT2_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 20);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_LCD1_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_LCD1_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 17);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP1_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_DFP1_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 19);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP2_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_DFP2_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 23);
	}
	if (radeon_encoder->devices & ATOM_DEVICE_DFP3_SUPPORT) {
		bios_3_scratch &= ~ATOM_S3_DFP3_CRTC_ACTIVE;
		bios_3_scratch |= (crtc << 25);
	}

	if (rdev->family >= CHIP_R600)
		WREG32(R600_BIOS_3_SCRATCH, bios_3_scratch);
	else
		WREG32(RADEON_BIOS_3_SCRATCH, bios_3_scratch);
}
