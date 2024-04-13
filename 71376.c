process_fw_state_change_wq(struct work_struct *work)
{
	struct megasas_instance *instance =
		container_of(work, struct megasas_instance, work_init);
	u32 wait;
	unsigned long flags;

    if (atomic_read(&instance->adprecovery) != MEGASAS_ADPRESET_SM_INFAULT) {
		dev_notice(&instance->pdev->dev, "error, recovery st %x\n",
				atomic_read(&instance->adprecovery));
		return ;
	}

	if (atomic_read(&instance->adprecovery) == MEGASAS_ADPRESET_SM_INFAULT) {
		dev_notice(&instance->pdev->dev, "FW detected to be in fault"
					"state, restarting it...\n");

		instance->instancet->disable_intr(instance);
		atomic_set(&instance->fw_outstanding, 0);

		atomic_set(&instance->fw_reset_no_pci_access, 1);
		instance->instancet->adp_reset(instance, instance->reg_set);
		atomic_set(&instance->fw_reset_no_pci_access, 0);

		dev_notice(&instance->pdev->dev, "FW restarted successfully,"
					"initiating next stage...\n");

		dev_notice(&instance->pdev->dev, "HBA recovery state machine,"
					"state 2 starting...\n");

		/* waiting for about 20 second before start the second init */
		for (wait = 0; wait < 30; wait++) {
			msleep(1000);
		}

		if (megasas_transition_to_ready(instance, 1)) {
			dev_notice(&instance->pdev->dev, "adapter not ready\n");

			atomic_set(&instance->fw_reset_no_pci_access, 1);
			megaraid_sas_kill_hba(instance);
			return ;
		}

		if ((instance->pdev->device == PCI_DEVICE_ID_LSI_SAS1064R) ||
			(instance->pdev->device == PCI_DEVICE_ID_DELL_PERC5) ||
			(instance->pdev->device == PCI_DEVICE_ID_LSI_VERDE_ZCR)
			) {
			*instance->consumer = *instance->producer;
		} else {
			*instance->consumer = 0;
			*instance->producer = 0;
		}

		megasas_issue_init_mfi(instance);

		spin_lock_irqsave(&instance->hba_lock, flags);
		atomic_set(&instance->adprecovery, MEGASAS_HBA_OPERATIONAL);
		spin_unlock_irqrestore(&instance->hba_lock, flags);
		instance->instancet->enable_intr(instance);

		megasas_issue_pending_cmds_again(instance);
		instance->issuepend_done = 1;
	}
}
