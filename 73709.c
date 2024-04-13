radeon_atombios_connected_scratch_regs(struct drm_connector *connector,
				       struct drm_encoder *encoder,
				       bool connected)
{
	struct drm_device *dev = connector->dev;
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_connector *radeon_connector =
	    to_radeon_connector(connector);
	struct radeon_encoder *radeon_encoder = to_radeon_encoder(encoder);
	uint32_t bios_0_scratch, bios_3_scratch, bios_6_scratch;

	if (rdev->family >= CHIP_R600) {
		bios_0_scratch = RREG32(R600_BIOS_0_SCRATCH);
		bios_3_scratch = RREG32(R600_BIOS_3_SCRATCH);
		bios_6_scratch = RREG32(R600_BIOS_6_SCRATCH);
	} else {
		bios_0_scratch = RREG32(RADEON_BIOS_0_SCRATCH);
		bios_3_scratch = RREG32(RADEON_BIOS_3_SCRATCH);
		bios_6_scratch = RREG32(RADEON_BIOS_6_SCRATCH);
	}

	if ((radeon_encoder->devices & ATOM_DEVICE_TV1_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_TV1_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("TV1 connected\n");
			bios_3_scratch |= ATOM_S3_TV1_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_TV1;
		} else {
			DRM_DEBUG("TV1 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_TV1_MASK;
			bios_3_scratch &= ~ATOM_S3_TV1_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_TV1;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_CV_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_CV_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("CV connected\n");
			bios_3_scratch |= ATOM_S3_CV_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_CV;
		} else {
			DRM_DEBUG("CV disconnected\n");
			bios_0_scratch &= ~ATOM_S0_CV_MASK;
			bios_3_scratch &= ~ATOM_S3_CV_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_CV;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_LCD1_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_LCD1_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("LCD1 connected\n");
			bios_0_scratch |= ATOM_S0_LCD1;
			bios_3_scratch |= ATOM_S3_LCD1_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_LCD1;
		} else {
			DRM_DEBUG("LCD1 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_LCD1;
			bios_3_scratch &= ~ATOM_S3_LCD1_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_LCD1;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_CRT1_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_CRT1_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("CRT1 connected\n");
			bios_0_scratch |= ATOM_S0_CRT1_COLOR;
			bios_3_scratch |= ATOM_S3_CRT1_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_CRT1;
		} else {
			DRM_DEBUG("CRT1 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_CRT1_MASK;
			bios_3_scratch &= ~ATOM_S3_CRT1_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_CRT1;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_CRT2_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_CRT2_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("CRT2 connected\n");
			bios_0_scratch |= ATOM_S0_CRT2_COLOR;
			bios_3_scratch |= ATOM_S3_CRT2_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_CRT2;
		} else {
			DRM_DEBUG("CRT2 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_CRT2_MASK;
			bios_3_scratch &= ~ATOM_S3_CRT2_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_CRT2;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_DFP1_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_DFP1_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("DFP1 connected\n");
			bios_0_scratch |= ATOM_S0_DFP1;
			bios_3_scratch |= ATOM_S3_DFP1_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_DFP1;
		} else {
			DRM_DEBUG("DFP1 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_DFP1;
			bios_3_scratch &= ~ATOM_S3_DFP1_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_DFP1;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_DFP2_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_DFP2_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("DFP2 connected\n");
			bios_0_scratch |= ATOM_S0_DFP2;
			bios_3_scratch |= ATOM_S3_DFP2_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_DFP2;
		} else {
			DRM_DEBUG("DFP2 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_DFP2;
			bios_3_scratch &= ~ATOM_S3_DFP2_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_DFP2;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_DFP3_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_DFP3_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("DFP3 connected\n");
			bios_0_scratch |= ATOM_S0_DFP3;
			bios_3_scratch |= ATOM_S3_DFP3_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_DFP3;
		} else {
			DRM_DEBUG("DFP3 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_DFP3;
			bios_3_scratch &= ~ATOM_S3_DFP3_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_DFP3;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_DFP4_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_DFP4_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("DFP4 connected\n");
			bios_0_scratch |= ATOM_S0_DFP4;
			bios_3_scratch |= ATOM_S3_DFP4_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_DFP4;
		} else {
			DRM_DEBUG("DFP4 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_DFP4;
			bios_3_scratch &= ~ATOM_S3_DFP4_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_DFP4;
		}
	}
	if ((radeon_encoder->devices & ATOM_DEVICE_DFP5_SUPPORT) &&
	    (radeon_connector->devices & ATOM_DEVICE_DFP5_SUPPORT)) {
		if (connected) {
			DRM_DEBUG("DFP5 connected\n");
			bios_0_scratch |= ATOM_S0_DFP5;
			bios_3_scratch |= ATOM_S3_DFP5_ACTIVE;
			bios_6_scratch |= ATOM_S6_ACC_REQ_DFP5;
		} else {
			DRM_DEBUG("DFP5 disconnected\n");
			bios_0_scratch &= ~ATOM_S0_DFP5;
			bios_3_scratch &= ~ATOM_S3_DFP5_ACTIVE;
			bios_6_scratch &= ~ATOM_S6_ACC_REQ_DFP5;
		}
	}

	if (rdev->family >= CHIP_R600) {
		WREG32(R600_BIOS_0_SCRATCH, bios_0_scratch);
		WREG32(R600_BIOS_3_SCRATCH, bios_3_scratch);
		WREG32(R600_BIOS_6_SCRATCH, bios_6_scratch);
	} else {
		WREG32(RADEON_BIOS_0_SCRATCH, bios_0_scratch);
		WREG32(RADEON_BIOS_3_SCRATCH, bios_3_scratch);
		WREG32(RADEON_BIOS_6_SCRATCH, bios_6_scratch);
	}
}
