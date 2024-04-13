megasas_suspend(struct pci_dev *pdev, pm_message_t state)
{
	struct Scsi_Host *host;
	struct megasas_instance *instance;

	instance = pci_get_drvdata(pdev);
	host = instance->host;
	instance->unload = 1;

	/* Shutdown SR-IOV heartbeat timer */
	if (instance->requestorId && !instance->skip_heartbeat_timer_del)
		del_timer_sync(&instance->sriov_heartbeat_timer);

	/* Stop the FW fault detection watchdog */
	if (instance->adapter_type != MFI_SERIES)
		megasas_fusion_stop_watchdog(instance);

	megasas_flush_cache(instance);
	megasas_shutdown_controller(instance, MR_DCMD_HIBERNATE_SHUTDOWN);

	/* cancel the delayed work if this work still in queue */
	if (instance->ev != NULL) {
		struct megasas_aen_event *ev = instance->ev;
		cancel_delayed_work_sync(&ev->hotplug_work);
		instance->ev = NULL;
	}

	tasklet_kill(&instance->isr_tasklet);

	pci_set_drvdata(instance->pdev, instance);
	instance->instancet->disable_intr(instance);

	megasas_destroy_irqs(instance);

	if (instance->msix_vectors)
		pci_free_irq_vectors(instance->pdev);

	pci_save_state(pdev);
	pci_disable_device(pdev);

	pci_set_power_state(pdev, pci_choose_state(pdev, state));

	return 0;
}
