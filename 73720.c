bool radeon_get_atom_connector_info_from_supported_devices_table(struct
								 drm_device
								 *dev)
{
	struct radeon_device *rdev = dev->dev_private;
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	struct atom_context *ctx = mode_info->atom_context;
	int index = GetIndexIntoMasterTable(DATA, SupportedDevicesInfo);
	uint16_t size, data_offset;
	uint8_t frev, crev;
	uint16_t device_support;
	uint8_t dac;
	union atom_supported_devices *supported_devices;
	int i, j, max_device;
	struct bios_connector bios_connectors[ATOM_MAX_SUPPORTED_DEVICE];

	if (!atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset))
		return false;

	supported_devices =
	    (union atom_supported_devices *)(ctx->bios + data_offset);

	device_support = le16_to_cpu(supported_devices->info.usDeviceSupport);

	if (frev > 1)
		max_device = ATOM_MAX_SUPPORTED_DEVICE;
	else
		max_device = ATOM_MAX_SUPPORTED_DEVICE_INFO;

	for (i = 0; i < max_device; i++) {
		ATOM_CONNECTOR_INFO_I2C ci =
		    supported_devices->info.asConnInfo[i];

		bios_connectors[i].valid = false;

		if (!(device_support & (1 << i))) {
			continue;
		}

		if (i == ATOM_DEVICE_CV_INDEX) {
			DRM_DEBUG("Skipping Component Video\n");
			continue;
		}

		bios_connectors[i].connector_type =
		    supported_devices_connector_convert[ci.sucConnectorInfo.
							sbfAccess.
							bfConnectorType];

		if (bios_connectors[i].connector_type ==
		    DRM_MODE_CONNECTOR_Unknown)
			continue;

		dac = ci.sucConnectorInfo.sbfAccess.bfAssociatedDAC;

		bios_connectors[i].line_mux =
			ci.sucI2cId.ucAccess;

		/* give tv unique connector ids */
		if (i == ATOM_DEVICE_TV1_INDEX) {
			bios_connectors[i].ddc_bus.valid = false;
			bios_connectors[i].line_mux = 50;
		} else if (i == ATOM_DEVICE_TV2_INDEX) {
			bios_connectors[i].ddc_bus.valid = false;
			bios_connectors[i].line_mux = 51;
		} else if (i == ATOM_DEVICE_CV_INDEX) {
			bios_connectors[i].ddc_bus.valid = false;
			bios_connectors[i].line_mux = 52;
		} else
			bios_connectors[i].ddc_bus =
			    radeon_lookup_i2c_gpio(rdev,
						   bios_connectors[i].line_mux);

		if ((crev > 1) && (frev > 1)) {
			u8 isb = supported_devices->info_2d1.asIntSrcInfo[i].ucIntSrcBitmap;
			switch (isb) {
			case 0x4:
				bios_connectors[i].hpd.hpd = RADEON_HPD_1;
				break;
			case 0xa:
				bios_connectors[i].hpd.hpd = RADEON_HPD_2;
				break;
			default:
				bios_connectors[i].hpd.hpd = RADEON_HPD_NONE;
				break;
			}
		} else {
			if (i == ATOM_DEVICE_DFP1_INDEX)
				bios_connectors[i].hpd.hpd = RADEON_HPD_1;
			else if (i == ATOM_DEVICE_DFP2_INDEX)
				bios_connectors[i].hpd.hpd = RADEON_HPD_2;
			else
				bios_connectors[i].hpd.hpd = RADEON_HPD_NONE;
		}

		/* Always set the connector type to VGA for CRT1/CRT2. if they are
		 * shared with a DVI port, we'll pick up the DVI connector when we
		 * merge the outputs.  Some bioses incorrectly list VGA ports as DVI.
		 */
		if (i == ATOM_DEVICE_CRT1_INDEX || i == ATOM_DEVICE_CRT2_INDEX)
			bios_connectors[i].connector_type =
			    DRM_MODE_CONNECTOR_VGA;

		if (!radeon_atom_apply_quirks
		    (dev, (1 << i), &bios_connectors[i].connector_type,
		     &bios_connectors[i].ddc_bus, &bios_connectors[i].line_mux,
		     &bios_connectors[i].hpd))
			continue;

		bios_connectors[i].valid = true;
		bios_connectors[i].devices = (1 << i);

		if (ASIC_IS_AVIVO(rdev) || radeon_r4xx_atom)
			radeon_add_atom_encoder(dev,
						radeon_get_encoder_id(dev,
								      (1 << i),
								      dac),
						(1 << i));
		else
			radeon_add_legacy_encoder(dev,
						  radeon_get_encoder_id(dev,
									(1 << i),
									dac),
						  (1 << i));
	}

	/* combine shared connectors */
	for (i = 0; i < max_device; i++) {
		if (bios_connectors[i].valid) {
			for (j = 0; j < max_device; j++) {
				if (bios_connectors[j].valid && (i != j)) {
					if (bios_connectors[i].line_mux ==
					    bios_connectors[j].line_mux) {
						/* make sure not to combine LVDS */
						if (bios_connectors[i].devices & (ATOM_DEVICE_LCD_SUPPORT)) {
							bios_connectors[i].line_mux = 53;
							bios_connectors[i].ddc_bus.valid = false;
							continue;
						}
						if (bios_connectors[j].devices & (ATOM_DEVICE_LCD_SUPPORT)) {
							bios_connectors[j].line_mux = 53;
							bios_connectors[j].ddc_bus.valid = false;
							continue;
						}
						/* combine analog and digital for DVI-I */
						if (((bios_connectors[i].devices & (ATOM_DEVICE_DFP_SUPPORT)) &&
						     (bios_connectors[j].devices & (ATOM_DEVICE_CRT_SUPPORT))) ||
						    ((bios_connectors[j].devices & (ATOM_DEVICE_DFP_SUPPORT)) &&
						     (bios_connectors[i].devices & (ATOM_DEVICE_CRT_SUPPORT)))) {
							bios_connectors[i].devices |=
								bios_connectors[j].devices;
							bios_connectors[i].connector_type =
								DRM_MODE_CONNECTOR_DVII;
							if (bios_connectors[j].devices & (ATOM_DEVICE_DFP_SUPPORT))
								bios_connectors[i].hpd =
									bios_connectors[j].hpd;
							bios_connectors[j].valid = false;
						}
					}
				}
			}
		}
	}

	/* add the connectors */
	for (i = 0; i < max_device; i++) {
		if (bios_connectors[i].valid) {
			uint16_t connector_object_id =
				atombios_get_connector_object_id(dev,
						      bios_connectors[i].connector_type,
						      bios_connectors[i].devices);
			radeon_add_atom_connector(dev,
						  bios_connectors[i].line_mux,
						  bios_connectors[i].devices,
						  bios_connectors[i].
						  connector_type,
						  &bios_connectors[i].ddc_bus,
						  false, 0,
						  connector_object_id,
						  &bios_connectors[i].hpd);
		}
	}

	radeon_link_encoder_connector(dev);

	return true;
}
