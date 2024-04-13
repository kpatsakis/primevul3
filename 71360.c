megasas_setup_irqs_msix(struct megasas_instance *instance, u8 is_probe)
{
	int i, j;
	struct pci_dev *pdev;

	pdev = instance->pdev;

	/* Try MSI-x */
	for (i = 0; i < instance->msix_vectors; i++) {
		instance->irq_context[i].instance = instance;
		instance->irq_context[i].MSIxIndex = i;
		if (request_irq(pci_irq_vector(pdev, i),
			instance->instancet->service_isr, 0, "megasas",
			&instance->irq_context[i])) {
			dev_err(&instance->pdev->dev,
				"Failed to register IRQ for vector %d.\n", i);
			for (j = 0; j < i; j++)
				free_irq(pci_irq_vector(pdev, j),
					 &instance->irq_context[j]);
			/* Retry irq register for IO_APIC*/
			instance->msix_vectors = 0;
			if (is_probe) {
				pci_free_irq_vectors(instance->pdev);
				return megasas_setup_irqs_ioapic(instance);
			} else {
				return -1;
			}
		}
	}
	return 0;
}
