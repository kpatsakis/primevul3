megasas_adp_reset_xscale(struct megasas_instance *instance,
	struct megasas_register_set __iomem *regs)
{
	u32 i;
	u32 pcidata;

	writel(MFI_ADP_RESET, &regs->inbound_doorbell);

	for (i = 0; i < 3; i++)
		msleep(1000); /* sleep for 3 secs */
	pcidata  = 0;
	pci_read_config_dword(instance->pdev, MFI_1068_PCSR_OFFSET, &pcidata);
	dev_notice(&instance->pdev->dev, "pcidata = %x\n", pcidata);
	if (pcidata & 0x2) {
		dev_notice(&instance->pdev->dev, "mfi 1068 offset read=%x\n", pcidata);
		pcidata &= ~0x2;
		pci_write_config_dword(instance->pdev,
				MFI_1068_PCSR_OFFSET, pcidata);

		for (i = 0; i < 2; i++)
			msleep(1000); /* need to wait 2 secs again */

		pcidata  = 0;
		pci_read_config_dword(instance->pdev,
				MFI_1068_FW_HANDSHAKE_OFFSET, &pcidata);
		dev_notice(&instance->pdev->dev, "1068 offset handshake read=%x\n", pcidata);
		if ((pcidata & 0xffff0000) == MFI_1068_FW_READY) {
			dev_notice(&instance->pdev->dev, "1068 offset pcidt=%x\n", pcidata);
			pcidata = 0;
			pci_write_config_dword(instance->pdev,
				MFI_1068_FW_HANDSHAKE_OFFSET, pcidata);
		}
	}
	return 0;
}
