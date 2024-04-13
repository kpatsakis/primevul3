megasas_adp_reset_gen2(struct megasas_instance *instance,
			struct megasas_register_set __iomem *reg_set)
{
	u32 retry = 0 ;
	u32 HostDiag;
	u32 __iomem *seq_offset = &reg_set->seq_offset;
	u32 __iomem *hostdiag_offset = &reg_set->host_diag;

	if (instance->instancet == &megasas_instance_template_skinny) {
		seq_offset = &reg_set->fusion_seq_offset;
		hostdiag_offset = &reg_set->fusion_host_diag;
	}

	writel(0, seq_offset);
	writel(4, seq_offset);
	writel(0xb, seq_offset);
	writel(2, seq_offset);
	writel(7, seq_offset);
	writel(0xd, seq_offset);

	msleep(1000);

	HostDiag = (u32)readl(hostdiag_offset);

	while (!(HostDiag & DIAG_WRITE_ENABLE)) {
		msleep(100);
		HostDiag = (u32)readl(hostdiag_offset);
		dev_notice(&instance->pdev->dev, "RESETGEN2: retry=%x, hostdiag=%x\n",
					retry, HostDiag);

		if (retry++ >= 100)
			return 1;

	}

	dev_notice(&instance->pdev->dev, "ADP_RESET_GEN2: HostDiag=%x\n", HostDiag);

	writel((HostDiag | DIAG_RESET_ADAPTER), hostdiag_offset);

	ssleep(10);

	HostDiag = (u32)readl(hostdiag_offset);
	while (HostDiag & DIAG_RESET_ADAPTER) {
		msleep(100);
		HostDiag = (u32)readl(hostdiag_offset);
		dev_notice(&instance->pdev->dev, "RESET_GEN2: retry=%x, hostdiag=%x\n",
				retry, HostDiag);

		if (retry++ >= 1000)
			return 1;

	}
	return 0;
}
