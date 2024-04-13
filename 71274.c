megasas_destroy_irqs(struct megasas_instance *instance) {

	int i;

	if (instance->msix_vectors)
		for (i = 0; i < instance->msix_vectors; i++) {
			free_irq(pci_irq_vector(instance->pdev, i),
				 &instance->irq_context[i]);
		}
	else
		free_irq(pci_irq_vector(instance->pdev, 0),
			 &instance->irq_context[0]);
}
