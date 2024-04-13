megasas_resume(struct pci_dev *pdev)
{
	int rval;
	struct Scsi_Host *host;
	struct megasas_instance *instance;
	int irq_flags = PCI_IRQ_LEGACY;

	instance = pci_get_drvdata(pdev);
	host = instance->host;
	pci_set_power_state(pdev, PCI_D0);
	pci_enable_wake(pdev, PCI_D0, 0);
	pci_restore_state(pdev);

	/*
	 * PCI prepping: enable device set bus mastering and dma mask
	 */
	rval = pci_enable_device_mem(pdev);

	if (rval) {
		dev_err(&pdev->dev, "Enable device failed\n");
		return rval;
	}

	pci_set_master(pdev);

	/*
	 * We expect the FW state to be READY
	 */
	if (megasas_transition_to_ready(instance, 0))
		goto fail_ready_state;

	if (megasas_set_dma_mask(instance))
		goto fail_set_dma_mask;

	/*
	 * Initialize MFI Firmware
	 */

	atomic_set(&instance->fw_outstanding, 0);
	atomic_set(&instance->ldio_outstanding, 0);

	/* Now re-enable MSI-X */
	if (instance->msix_vectors) {
		irq_flags = PCI_IRQ_MSIX;
		if (smp_affinity_enable)
			irq_flags |= PCI_IRQ_AFFINITY;
	}
	rval = pci_alloc_irq_vectors(instance->pdev, 1,
				     instance->msix_vectors ?
				     instance->msix_vectors : 1, irq_flags);
	if (rval < 0)
		goto fail_reenable_msix;

	megasas_setup_reply_map(instance);

	if (instance->adapter_type != MFI_SERIES) {
		megasas_reset_reply_desc(instance);
		if (megasas_ioc_init_fusion(instance)) {
			megasas_free_cmds(instance);
			megasas_free_cmds_fusion(instance);
			goto fail_init_mfi;
		}
		if (!megasas_get_map_info(instance))
			megasas_sync_map_info(instance);
	} else {
		*instance->producer = 0;
		*instance->consumer = 0;
		if (megasas_issue_init_mfi(instance))
			goto fail_init_mfi;
	}

	if (megasas_get_ctrl_info(instance) != DCMD_SUCCESS)
		goto fail_init_mfi;

	tasklet_init(&instance->isr_tasklet, instance->instancet->tasklet,
		     (unsigned long)instance);

	if (instance->msix_vectors ?
			megasas_setup_irqs_msix(instance, 0) :
			megasas_setup_irqs_ioapic(instance))
		goto fail_init_mfi;

	/* Re-launch SR-IOV heartbeat timer */
	if (instance->requestorId) {
		if (!megasas_sriov_start_heartbeat(instance, 0))
			megasas_start_timer(instance);
		else {
			instance->skip_heartbeat_timer_del = 1;
			goto fail_init_mfi;
		}
	}

	instance->instancet->enable_intr(instance);
	megasas_setup_jbod_map(instance);
	instance->unload = 0;

	/*
	 * Initiate AEN (Asynchronous Event Notification)
	 */
	if (megasas_start_aen(instance))
		dev_err(&instance->pdev->dev, "Start AEN failed\n");

	/* Re-launch FW fault watchdog */
	if (instance->adapter_type != MFI_SERIES)
		if (megasas_fusion_start_watchdog(instance) != SUCCESS)
			goto fail_start_watchdog;

	return 0;

fail_start_watchdog:
	if (instance->requestorId && !instance->skip_heartbeat_timer_del)
		del_timer_sync(&instance->sriov_heartbeat_timer);
fail_init_mfi:
	megasas_free_ctrl_dma_buffers(instance);
	megasas_free_ctrl_mem(instance);
	scsi_host_put(host);

fail_reenable_msix:
fail_set_dma_mask:
fail_ready_state:

	pci_disable_device(pdev);

	return -ENODEV;
}
