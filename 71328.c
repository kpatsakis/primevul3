megasas_make_sgl64(struct megasas_instance *instance, struct scsi_cmnd *scp,
		   union megasas_sgl *mfi_sgl)
{
	int i;
	int sge_count;
	struct scatterlist *os_sgl;

	sge_count = scsi_dma_map(scp);
	BUG_ON(sge_count < 0);

	if (sge_count) {
		scsi_for_each_sg(scp, os_sgl, sge_count, i) {
			mfi_sgl->sge64[i].length = cpu_to_le32(sg_dma_len(os_sgl));
			mfi_sgl->sge64[i].phys_addr = cpu_to_le64(sg_dma_address(os_sgl));
		}
	}
	return sge_count;
}
