megasas_deplete_reply_queue(struct megasas_instance *instance,
					u8 alt_status)
{
	u32 mfiStatus;
	u32 fw_state;

	if ((mfiStatus = instance->instancet->check_reset(instance,
					instance->reg_set)) == 1) {
		return IRQ_HANDLED;
	}

	mfiStatus = instance->instancet->clear_intr(instance);
	if (mfiStatus == 0) {
		/* Hardware may not set outbound_intr_status in MSI-X mode */
		if (!instance->msix_vectors)
			return IRQ_NONE;
	}

	instance->mfiStatus = mfiStatus;

	if ((mfiStatus & MFI_INTR_FLAG_FIRMWARE_STATE_CHANGE)) {
		fw_state = instance->instancet->read_fw_status_reg(
				instance) & MFI_STATE_MASK;

		if (fw_state != MFI_STATE_FAULT) {
			dev_notice(&instance->pdev->dev, "fw state:%x\n",
						fw_state);
		}

		if ((fw_state == MFI_STATE_FAULT) &&
				(instance->disableOnlineCtrlReset == 0)) {
			dev_notice(&instance->pdev->dev, "wait adp restart\n");

			if ((instance->pdev->device ==
					PCI_DEVICE_ID_LSI_SAS1064R) ||
				(instance->pdev->device ==
					PCI_DEVICE_ID_DELL_PERC5) ||
				(instance->pdev->device ==
					PCI_DEVICE_ID_LSI_VERDE_ZCR)) {

				*instance->consumer =
					cpu_to_le32(MEGASAS_ADPRESET_INPROG_SIGN);
			}


			instance->instancet->disable_intr(instance);
			atomic_set(&instance->adprecovery, MEGASAS_ADPRESET_SM_INFAULT);
			instance->issuepend_done = 0;

			atomic_set(&instance->fw_outstanding, 0);
			megasas_internal_reset_defer_cmds(instance);

			dev_notice(&instance->pdev->dev, "fwState=%x, stage:%d\n",
					fw_state, atomic_read(&instance->adprecovery));

			schedule_work(&instance->work_init);
			return IRQ_HANDLED;

		} else {
			dev_notice(&instance->pdev->dev, "fwstate:%x, dis_OCR=%x\n",
				fw_state, instance->disableOnlineCtrlReset);
		}
	}

	tasklet_schedule(&instance->isr_tasklet);
	return IRQ_HANDLED;
}
