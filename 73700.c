static uint16_t atombios_get_connector_object_id(struct drm_device *dev,
						 int connector_type,
						 uint16_t devices)
{
	struct radeon_device *rdev = dev->dev_private;

	if (rdev->flags & RADEON_IS_IGP) {
		return supported_devices_connector_object_id_convert
			[connector_type];
	} else if (((connector_type == DRM_MODE_CONNECTOR_DVII) ||
		    (connector_type == DRM_MODE_CONNECTOR_DVID)) &&
		   (devices & ATOM_DEVICE_DFP2_SUPPORT))  {
		struct radeon_mode_info *mode_info = &rdev->mode_info;
		struct atom_context *ctx = mode_info->atom_context;
		int index = GetIndexIntoMasterTable(DATA, XTMDS_Info);
		uint16_t size, data_offset;
		uint8_t frev, crev;
		ATOM_XTMDS_INFO *xtmds;

		if (atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset)) {
			xtmds = (ATOM_XTMDS_INFO *)(ctx->bios + data_offset);

			if (xtmds->ucSupportedLink & ATOM_XTMDS_SUPPORTED_DUALLINK) {
				if (connector_type == DRM_MODE_CONNECTOR_DVII)
					return CONNECTOR_OBJECT_ID_DUAL_LINK_DVI_I;
				else
					return CONNECTOR_OBJECT_ID_DUAL_LINK_DVI_D;
			} else {
				if (connector_type == DRM_MODE_CONNECTOR_DVII)
					return CONNECTOR_OBJECT_ID_SINGLE_LINK_DVI_I;
				else
					return CONNECTOR_OBJECT_ID_SINGLE_LINK_DVI_D;
			}
		} else
			return supported_devices_connector_object_id_convert
				[connector_type];
	} else {
		return supported_devices_connector_object_id_convert
			[connector_type];
	}
}
