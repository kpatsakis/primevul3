megasas_transition_to_ready(struct megasas_instance *instance, int ocr)
{
	int i;
	u8 max_wait;
	u32 fw_state;
	u32 cur_state;
	u32 abs_state, curr_abs_state;

	abs_state = instance->instancet->read_fw_status_reg(instance);
	fw_state = abs_state & MFI_STATE_MASK;

	if (fw_state != MFI_STATE_READY)
		dev_info(&instance->pdev->dev, "Waiting for FW to come to ready"
		       " state\n");

	while (fw_state != MFI_STATE_READY) {

		switch (fw_state) {

		case MFI_STATE_FAULT:
			dev_printk(KERN_DEBUG, &instance->pdev->dev, "FW in FAULT state!!\n");
			if (ocr) {
				max_wait = MEGASAS_RESET_WAIT_TIME;
				cur_state = MFI_STATE_FAULT;
				break;
			} else
				return -ENODEV;

		case MFI_STATE_WAIT_HANDSHAKE:
			/*
			 * Set the CLR bit in inbound doorbell
			 */
			if ((instance->pdev->device ==
				PCI_DEVICE_ID_LSI_SAS0073SKINNY) ||
				(instance->pdev->device ==
				 PCI_DEVICE_ID_LSI_SAS0071SKINNY) ||
				(instance->adapter_type != MFI_SERIES))
				writel(
				  MFI_INIT_CLEAR_HANDSHAKE|MFI_INIT_HOTPLUG,
				  &instance->reg_set->doorbell);
			else
				writel(
				    MFI_INIT_CLEAR_HANDSHAKE|MFI_INIT_HOTPLUG,
					&instance->reg_set->inbound_doorbell);

			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_WAIT_HANDSHAKE;
			break;

		case MFI_STATE_BOOT_MESSAGE_PENDING:
			if ((instance->pdev->device ==
			     PCI_DEVICE_ID_LSI_SAS0073SKINNY) ||
				(instance->pdev->device ==
				 PCI_DEVICE_ID_LSI_SAS0071SKINNY) ||
				(instance->adapter_type != MFI_SERIES))
				writel(MFI_INIT_HOTPLUG,
				       &instance->reg_set->doorbell);
			else
				writel(MFI_INIT_HOTPLUG,
					&instance->reg_set->inbound_doorbell);

			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_BOOT_MESSAGE_PENDING;
			break;

		case MFI_STATE_OPERATIONAL:
			/*
			 * Bring it to READY state; assuming max wait 10 secs
			 */
			instance->instancet->disable_intr(instance);
			if ((instance->pdev->device ==
				PCI_DEVICE_ID_LSI_SAS0073SKINNY) ||
				(instance->pdev->device ==
				PCI_DEVICE_ID_LSI_SAS0071SKINNY)  ||
				(instance->adapter_type != MFI_SERIES)) {
				writel(MFI_RESET_FLAGS,
					&instance->reg_set->doorbell);

				if (instance->adapter_type != MFI_SERIES) {
					for (i = 0; i < (10 * 1000); i += 20) {
						if (megasas_readl(
							    instance,
							    &instance->
							    reg_set->
							    doorbell) & 1)
							msleep(20);
						else
							break;
					}
				}
			} else
				writel(MFI_RESET_FLAGS,
					&instance->reg_set->inbound_doorbell);

			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_OPERATIONAL;
			break;

		case MFI_STATE_UNDEFINED:
			/*
			 * This state should not last for more than 2 seconds
			 */
			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_UNDEFINED;
			break;

		case MFI_STATE_BB_INIT:
			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_BB_INIT;
			break;

		case MFI_STATE_FW_INIT:
			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_FW_INIT;
			break;

		case MFI_STATE_FW_INIT_2:
			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_FW_INIT_2;
			break;

		case MFI_STATE_DEVICE_SCAN:
			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_DEVICE_SCAN;
			break;

		case MFI_STATE_FLUSH_CACHE:
			max_wait = MEGASAS_RESET_WAIT_TIME;
			cur_state = MFI_STATE_FLUSH_CACHE;
			break;

		default:
			dev_printk(KERN_DEBUG, &instance->pdev->dev, "Unknown state 0x%x\n",
			       fw_state);
			return -ENODEV;
		}

		/*
		 * The cur_state should not last for more than max_wait secs
		 */
		for (i = 0; i < max_wait; i++) {
			curr_abs_state = instance->instancet->
				read_fw_status_reg(instance);

			if (abs_state == curr_abs_state) {
				msleep(1000);
			} else
				break;
		}

		/*
		 * Return error if fw_state hasn't changed after max_wait
		 */
		if (curr_abs_state == abs_state) {
			dev_printk(KERN_DEBUG, &instance->pdev->dev, "FW state [%d] hasn't changed "
			       "in %d secs\n", fw_state, max_wait);
			return -ENODEV;
		}

		abs_state = curr_abs_state;
		fw_state = curr_abs_state & MFI_STATE_MASK;
	}
	dev_info(&instance->pdev->dev, "FW now in Ready state\n");

	return 0;
}
