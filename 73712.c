void radeon_atombios_get_power_modes(struct radeon_device *rdev)
{
	struct radeon_mode_info *mode_info = &rdev->mode_info;
	int index = GetIndexIntoMasterTable(DATA, PowerPlayInfo);
	u16 data_offset;
	u8 frev, crev;
	u32 misc, misc2 = 0, sclk, mclk;
	union power_info *power_info;
	struct _ATOM_PPLIB_NONCLOCK_INFO *non_clock_info;
	struct _ATOM_PPLIB_STATE *power_state;
	int num_modes = 0, i, j;
	int state_index = 0, mode_index = 0;
	struct radeon_i2c_bus_rec i2c_bus;

	rdev->pm.default_power_state = NULL;

	if (atom_parse_data_header(mode_info->atom_context, index, NULL,
				   &frev, &crev, &data_offset)) {
		power_info = (union power_info *)(mode_info->atom_context->bios + data_offset);
		if (frev < 4) {
			/* add the i2c bus for thermal/fan chip */
			if (power_info->info.ucOverdriveThermalController > 0) {
				DRM_INFO("Possible %s thermal controller at 0x%02x\n",
					 thermal_controller_names[power_info->info.ucOverdriveThermalController],
					 power_info->info.ucOverdriveControllerAddress >> 1);
				i2c_bus = radeon_lookup_i2c_gpio(rdev, power_info->info.ucOverdriveI2cLine);
				rdev->pm.i2c_bus = radeon_i2c_create(rdev->ddev, &i2c_bus, "Thermal");
			}
			num_modes = power_info->info.ucNumOfPowerModeEntries;
			if (num_modes > ATOM_MAX_NUMBEROF_POWER_BLOCK)
				num_modes = ATOM_MAX_NUMBEROF_POWER_BLOCK;
			for (i = 0; i < num_modes; i++) {
				rdev->pm.power_state[state_index].clock_info[0].voltage.type = VOLTAGE_NONE;
				switch (frev) {
				case 1:
					rdev->pm.power_state[state_index].num_clock_modes = 1;
					rdev->pm.power_state[state_index].clock_info[0].mclk =
						le16_to_cpu(power_info->info.asPowerPlayInfo[i].usMemoryClock);
					rdev->pm.power_state[state_index].clock_info[0].sclk =
						le16_to_cpu(power_info->info.asPowerPlayInfo[i].usEngineClock);
					/* skip invalid modes */
					if ((rdev->pm.power_state[state_index].clock_info[0].mclk == 0) ||
					    (rdev->pm.power_state[state_index].clock_info[0].sclk == 0))
						continue;
					/* skip overclock modes for now */
					if ((rdev->pm.power_state[state_index].clock_info[0].mclk >
					     rdev->clock.default_mclk + RADEON_MODE_OVERCLOCK_MARGIN) ||
					    (rdev->pm.power_state[state_index].clock_info[0].sclk >
					     rdev->clock.default_sclk + RADEON_MODE_OVERCLOCK_MARGIN))
						continue;
					rdev->pm.power_state[state_index].non_clock_info.pcie_lanes =
						power_info->info.asPowerPlayInfo[i].ucNumPciELanes;
					misc = le32_to_cpu(power_info->info.asPowerPlayInfo[i].ulMiscInfo);
					if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_SUPPORT) {
						rdev->pm.power_state[state_index].clock_info[0].voltage.type =
							VOLTAGE_GPIO;
						rdev->pm.power_state[state_index].clock_info[0].voltage.gpio =
							radeon_lookup_gpio(rdev,
							power_info->info.asPowerPlayInfo[i].ucVoltageDropIndex);
						if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_ACTIVE_HIGH)
							rdev->pm.power_state[state_index].clock_info[0].voltage.active_high =
								true;
						else
							rdev->pm.power_state[state_index].clock_info[0].voltage.active_high =
								false;
					} else if (misc & ATOM_PM_MISCINFO_PROGRAM_VOLTAGE) {
						rdev->pm.power_state[state_index].clock_info[0].voltage.type =
							VOLTAGE_VDDC;
						rdev->pm.power_state[state_index].clock_info[0].voltage.vddc_id =
							power_info->info.asPowerPlayInfo[i].ucVoltageDropIndex;
					}
					/* order matters! */
					if (misc & ATOM_PM_MISCINFO_POWER_SAVING_MODE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_POWERSAVE;
					if (misc & ATOM_PM_MISCINFO_DEFAULT_DC_STATE_ENTRY_TRUE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
					if (misc & ATOM_PM_MISCINFO_DEFAULT_LOW_DC_STATE_ENTRY_TRUE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
					if (misc & ATOM_PM_MISCINFO_LOAD_BALANCE_EN)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BALANCED;
					if (misc & ATOM_PM_MISCINFO_3D_ACCELERATION_EN)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_PERFORMANCE;
					if (misc & ATOM_PM_MISCINFO_DRIVER_DEFAULT_MODE) {
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_DEFAULT;
						rdev->pm.default_power_state = &rdev->pm.power_state[state_index];
						rdev->pm.power_state[state_index].default_clock_mode =
							&rdev->pm.power_state[state_index].clock_info[0];
					}
					state_index++;
					break;
				case 2:
					rdev->pm.power_state[state_index].num_clock_modes = 1;
					rdev->pm.power_state[state_index].clock_info[0].mclk =
						le32_to_cpu(power_info->info_2.asPowerPlayInfo[i].ulMemoryClock);
					rdev->pm.power_state[state_index].clock_info[0].sclk =
						le32_to_cpu(power_info->info_2.asPowerPlayInfo[i].ulEngineClock);
					/* skip invalid modes */
					if ((rdev->pm.power_state[state_index].clock_info[0].mclk == 0) ||
					    (rdev->pm.power_state[state_index].clock_info[0].sclk == 0))
						continue;
					/* skip overclock modes for now */
					if ((rdev->pm.power_state[state_index].clock_info[0].mclk >
					     rdev->clock.default_mclk + RADEON_MODE_OVERCLOCK_MARGIN) ||
					    (rdev->pm.power_state[state_index].clock_info[0].sclk >
					     rdev->clock.default_sclk + RADEON_MODE_OVERCLOCK_MARGIN))
						continue;
					rdev->pm.power_state[state_index].non_clock_info.pcie_lanes =
						power_info->info_2.asPowerPlayInfo[i].ucNumPciELanes;
					misc = le32_to_cpu(power_info->info_2.asPowerPlayInfo[i].ulMiscInfo);
					misc2 = le32_to_cpu(power_info->info_2.asPowerPlayInfo[i].ulMiscInfo2);
					if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_SUPPORT) {
						rdev->pm.power_state[state_index].clock_info[0].voltage.type =
							VOLTAGE_GPIO;
						rdev->pm.power_state[state_index].clock_info[0].voltage.gpio =
							radeon_lookup_gpio(rdev,
							power_info->info_2.asPowerPlayInfo[i].ucVoltageDropIndex);
						if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_ACTIVE_HIGH)
							rdev->pm.power_state[state_index].clock_info[0].voltage.active_high =
								true;
						else
							rdev->pm.power_state[state_index].clock_info[0].voltage.active_high =
								false;
					} else if (misc & ATOM_PM_MISCINFO_PROGRAM_VOLTAGE) {
						rdev->pm.power_state[state_index].clock_info[0].voltage.type =
							VOLTAGE_VDDC;
						rdev->pm.power_state[state_index].clock_info[0].voltage.vddc_id =
							power_info->info_2.asPowerPlayInfo[i].ucVoltageDropIndex;
					}
					/* order matters! */
					if (misc & ATOM_PM_MISCINFO_POWER_SAVING_MODE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_POWERSAVE;
					if (misc & ATOM_PM_MISCINFO_DEFAULT_DC_STATE_ENTRY_TRUE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
					if (misc & ATOM_PM_MISCINFO_DEFAULT_LOW_DC_STATE_ENTRY_TRUE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
					if (misc & ATOM_PM_MISCINFO_LOAD_BALANCE_EN)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BALANCED;
					if (misc & ATOM_PM_MISCINFO_3D_ACCELERATION_EN)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_PERFORMANCE;
					if (misc2 & ATOM_PM_MISCINFO2_SYSTEM_AC_LITE_MODE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BALANCED;
					if (misc & ATOM_PM_MISCINFO_DRIVER_DEFAULT_MODE) {
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_DEFAULT;
						rdev->pm.default_power_state = &rdev->pm.power_state[state_index];
						rdev->pm.power_state[state_index].default_clock_mode =
							&rdev->pm.power_state[state_index].clock_info[0];
					}
					state_index++;
					break;
				case 3:
					rdev->pm.power_state[state_index].num_clock_modes = 1;
					rdev->pm.power_state[state_index].clock_info[0].mclk =
						le32_to_cpu(power_info->info_3.asPowerPlayInfo[i].ulMemoryClock);
					rdev->pm.power_state[state_index].clock_info[0].sclk =
						le32_to_cpu(power_info->info_3.asPowerPlayInfo[i].ulEngineClock);
					/* skip invalid modes */
					if ((rdev->pm.power_state[state_index].clock_info[0].mclk == 0) ||
					    (rdev->pm.power_state[state_index].clock_info[0].sclk == 0))
						continue;
					/* skip overclock modes for now */
					if ((rdev->pm.power_state[state_index].clock_info[0].mclk >
					     rdev->clock.default_mclk + RADEON_MODE_OVERCLOCK_MARGIN) ||
					    (rdev->pm.power_state[state_index].clock_info[0].sclk >
					     rdev->clock.default_sclk + RADEON_MODE_OVERCLOCK_MARGIN))
						continue;
					rdev->pm.power_state[state_index].non_clock_info.pcie_lanes =
						power_info->info_3.asPowerPlayInfo[i].ucNumPciELanes;
					misc = le32_to_cpu(power_info->info_3.asPowerPlayInfo[i].ulMiscInfo);
					misc2 = le32_to_cpu(power_info->info_3.asPowerPlayInfo[i].ulMiscInfo2);
					if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_SUPPORT) {
						rdev->pm.power_state[state_index].clock_info[0].voltage.type =
							VOLTAGE_GPIO;
						rdev->pm.power_state[state_index].clock_info[0].voltage.gpio =
							radeon_lookup_gpio(rdev,
							power_info->info_3.asPowerPlayInfo[i].ucVoltageDropIndex);
						if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_ACTIVE_HIGH)
							rdev->pm.power_state[state_index].clock_info[0].voltage.active_high =
								true;
						else
							rdev->pm.power_state[state_index].clock_info[0].voltage.active_high =
								false;
					} else if (misc & ATOM_PM_MISCINFO_PROGRAM_VOLTAGE) {
						rdev->pm.power_state[state_index].clock_info[0].voltage.type =
							VOLTAGE_VDDC;
						rdev->pm.power_state[state_index].clock_info[0].voltage.vddc_id =
							power_info->info_3.asPowerPlayInfo[i].ucVoltageDropIndex;
						if (misc2 & ATOM_PM_MISCINFO2_VDDCI_DYNAMIC_VOLTAGE_EN) {
							rdev->pm.power_state[state_index].clock_info[0].voltage.vddci_enabled =
								true;
							rdev->pm.power_state[state_index].clock_info[0].voltage.vddci_id =
							power_info->info_3.asPowerPlayInfo[i].ucVDDCI_VoltageDropIndex;
						}
					}
					/* order matters! */
					if (misc & ATOM_PM_MISCINFO_POWER_SAVING_MODE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_POWERSAVE;
					if (misc & ATOM_PM_MISCINFO_DEFAULT_DC_STATE_ENTRY_TRUE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
					if (misc & ATOM_PM_MISCINFO_DEFAULT_LOW_DC_STATE_ENTRY_TRUE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
					if (misc & ATOM_PM_MISCINFO_LOAD_BALANCE_EN)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BALANCED;
					if (misc & ATOM_PM_MISCINFO_3D_ACCELERATION_EN)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_PERFORMANCE;
					if (misc2 & ATOM_PM_MISCINFO2_SYSTEM_AC_LITE_MODE)
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BALANCED;
					if (misc & ATOM_PM_MISCINFO_DRIVER_DEFAULT_MODE) {
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_DEFAULT;
						rdev->pm.default_power_state = &rdev->pm.power_state[state_index];
						rdev->pm.power_state[state_index].default_clock_mode =
							&rdev->pm.power_state[state_index].clock_info[0];
					}
					state_index++;
					break;
				}
			}
		} else if (frev == 4) {
			/* add the i2c bus for thermal/fan chip */
			/* no support for internal controller yet */
			if (power_info->info_4.sThermalController.ucType > 0) {
				if ((power_info->info_4.sThermalController.ucType == ATOM_PP_THERMALCONTROLLER_RV6xx) ||
				    (power_info->info_4.sThermalController.ucType == ATOM_PP_THERMALCONTROLLER_RV770)) {
					DRM_INFO("Internal thermal controller %s fan control\n",
						 (power_info->info_4.sThermalController.ucFanParameters &
						  ATOM_PP_FANPARAMETERS_NOFAN) ? "without" : "with");
				} else {
					DRM_INFO("Possible %s thermal controller at 0x%02x %s fan control\n",
						 pp_lib_thermal_controller_names[power_info->info_4.sThermalController.ucType],
						 power_info->info_4.sThermalController.ucI2cAddress >> 1,
						 (power_info->info_4.sThermalController.ucFanParameters &
						  ATOM_PP_FANPARAMETERS_NOFAN) ? "without" : "with");
					i2c_bus = radeon_lookup_i2c_gpio(rdev, power_info->info_4.sThermalController.ucI2cLine);
					rdev->pm.i2c_bus = radeon_i2c_create(rdev->ddev, &i2c_bus, "Thermal");
				}
			}
			for (i = 0; i < power_info->info_4.ucNumStates; i++) {
				mode_index = 0;
				power_state = (struct _ATOM_PPLIB_STATE *)
					(mode_info->atom_context->bios +
					 data_offset +
					 le16_to_cpu(power_info->info_4.usStateArrayOffset) +
					 i * power_info->info_4.ucStateEntrySize);
				non_clock_info = (struct _ATOM_PPLIB_NONCLOCK_INFO *)
					(mode_info->atom_context->bios +
					 data_offset +
					 le16_to_cpu(power_info->info_4.usNonClockInfoArrayOffset) +
					 (power_state->ucNonClockStateIndex *
					  power_info->info_4.ucNonClockSize));
				for (j = 0; j < (power_info->info_4.ucStateEntrySize - 1); j++) {
					if (rdev->flags & RADEON_IS_IGP) {
						struct _ATOM_PPLIB_RS780_CLOCK_INFO *clock_info =
							(struct _ATOM_PPLIB_RS780_CLOCK_INFO *)
							(mode_info->atom_context->bios +
							 data_offset +
							 le16_to_cpu(power_info->info_4.usClockInfoArrayOffset) +
							 (power_state->ucClockStateIndices[j] *
							  power_info->info_4.ucClockInfoSize));
						sclk = le16_to_cpu(clock_info->usLowEngineClockLow);
						sclk |= clock_info->ucLowEngineClockHigh << 16;
						rdev->pm.power_state[state_index].clock_info[mode_index].sclk = sclk;
						/* skip invalid modes */
						if (rdev->pm.power_state[state_index].clock_info[mode_index].sclk == 0)
							continue;
						/* skip overclock modes for now */
						if (rdev->pm.power_state[state_index].clock_info[mode_index].sclk >
						    rdev->clock.default_sclk + RADEON_MODE_OVERCLOCK_MARGIN)
							continue;
						rdev->pm.power_state[state_index].clock_info[mode_index].voltage.type =
							VOLTAGE_SW;
						rdev->pm.power_state[state_index].clock_info[mode_index].voltage.voltage =
							clock_info->usVDDC;
						mode_index++;
					} else {
						struct _ATOM_PPLIB_R600_CLOCK_INFO *clock_info =
							(struct _ATOM_PPLIB_R600_CLOCK_INFO *)
							(mode_info->atom_context->bios +
							 data_offset +
							 le16_to_cpu(power_info->info_4.usClockInfoArrayOffset) +
							 (power_state->ucClockStateIndices[j] *
							  power_info->info_4.ucClockInfoSize));
						sclk = le16_to_cpu(clock_info->usEngineClockLow);
						sclk |= clock_info->ucEngineClockHigh << 16;
						mclk = le16_to_cpu(clock_info->usMemoryClockLow);
						mclk |= clock_info->ucMemoryClockHigh << 16;
						rdev->pm.power_state[state_index].clock_info[mode_index].mclk = mclk;
						rdev->pm.power_state[state_index].clock_info[mode_index].sclk = sclk;
						/* skip invalid modes */
						if ((rdev->pm.power_state[state_index].clock_info[mode_index].mclk == 0) ||
						    (rdev->pm.power_state[state_index].clock_info[mode_index].sclk == 0))
							continue;
						/* skip overclock modes for now */
						if ((rdev->pm.power_state[state_index].clock_info[mode_index].mclk >
						     rdev->clock.default_mclk + RADEON_MODE_OVERCLOCK_MARGIN) ||
						    (rdev->pm.power_state[state_index].clock_info[mode_index].sclk >
						     rdev->clock.default_sclk + RADEON_MODE_OVERCLOCK_MARGIN))
							continue;
						rdev->pm.power_state[state_index].clock_info[mode_index].voltage.type =
							VOLTAGE_SW;
						rdev->pm.power_state[state_index].clock_info[mode_index].voltage.voltage =
							clock_info->usVDDC;
						mode_index++;
					}
				}
				rdev->pm.power_state[state_index].num_clock_modes = mode_index;
				if (mode_index) {
					misc = le32_to_cpu(non_clock_info->ulCapsAndSettings);
					misc2 = le16_to_cpu(non_clock_info->usClassification);
					rdev->pm.power_state[state_index].non_clock_info.pcie_lanes =
						((misc & ATOM_PPLIB_PCIE_LINK_WIDTH_MASK) >>
						ATOM_PPLIB_PCIE_LINK_WIDTH_SHIFT) + 1;
					switch (misc2 & ATOM_PPLIB_CLASSIFICATION_UI_MASK) {
					case ATOM_PPLIB_CLASSIFICATION_UI_BATTERY:
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BATTERY;
						break;
					case ATOM_PPLIB_CLASSIFICATION_UI_BALANCED:
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_BALANCED;
						break;
					case ATOM_PPLIB_CLASSIFICATION_UI_PERFORMANCE:
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_PERFORMANCE;
						break;
					}
					if (misc2 & ATOM_PPLIB_CLASSIFICATION_BOOT) {
						rdev->pm.power_state[state_index].type =
							POWER_STATE_TYPE_DEFAULT;
						rdev->pm.default_power_state = &rdev->pm.power_state[state_index];
						rdev->pm.power_state[state_index].default_clock_mode =
							&rdev->pm.power_state[state_index].clock_info[mode_index - 1];
					}
					state_index++;
				}
			}
		}
	} else {
		/* XXX figure out some good default low power mode for cards w/out power tables */
	}

	if (rdev->pm.default_power_state == NULL) {
		/* add the default mode */
		rdev->pm.power_state[state_index].type =
			POWER_STATE_TYPE_DEFAULT;
		rdev->pm.power_state[state_index].num_clock_modes = 1;
		rdev->pm.power_state[state_index].clock_info[0].mclk = rdev->clock.default_mclk;
		rdev->pm.power_state[state_index].clock_info[0].sclk = rdev->clock.default_sclk;
		rdev->pm.power_state[state_index].default_clock_mode =
			&rdev->pm.power_state[state_index].clock_info[0];
		rdev->pm.power_state[state_index].clock_info[0].voltage.type = VOLTAGE_NONE;
		if (rdev->asic->get_pcie_lanes)
			rdev->pm.power_state[state_index].non_clock_info.pcie_lanes = radeon_get_pcie_lanes(rdev);
		else
			rdev->pm.power_state[state_index].non_clock_info.pcie_lanes = 16;
		rdev->pm.default_power_state = &rdev->pm.power_state[state_index];
		state_index++;
	}
	rdev->pm.num_power_states = state_index;

	rdev->pm.current_power_state = rdev->pm.default_power_state;
	rdev->pm.current_clock_mode =
		rdev->pm.default_power_state->default_clock_mode;
}
