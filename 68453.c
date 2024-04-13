static int del_dac(struct task_struct *child, int slot)
{
	if (slot == 1) {
		if ((dbcr_dac(child) & (DBCR_DAC1R | DBCR_DAC1W)) == 0)
			return -ENOENT;

		child->thread.debug.dac1 = 0;
		dbcr_dac(child) &= ~(DBCR_DAC1R | DBCR_DAC1W);
#ifdef CONFIG_PPC_ADV_DEBUG_DAC_RANGE
		if (child->thread.debug.dbcr2 & DBCR2_DAC12MODE) {
			child->thread.debug.dac2 = 0;
			child->thread.debug.dbcr2 &= ~DBCR2_DAC12MODE;
		}
		child->thread.debug.dbcr2 &= ~(DBCR2_DVC1M | DBCR2_DVC1BE);
#endif
#if CONFIG_PPC_ADV_DEBUG_DVCS > 0
		child->thread.debug.dvc1 = 0;
#endif
	} else if (slot == 2) {
		if ((dbcr_dac(child) & (DBCR_DAC2R | DBCR_DAC2W)) == 0)
			return -ENOENT;

#ifdef CONFIG_PPC_ADV_DEBUG_DAC_RANGE
		if (child->thread.debug.dbcr2 & DBCR2_DAC12MODE)
			/* Part of a range */
			return -EINVAL;
		child->thread.debug.dbcr2 &= ~(DBCR2_DVC2M | DBCR2_DVC2BE);
#endif
#if CONFIG_PPC_ADV_DEBUG_DVCS > 0
		child->thread.debug.dvc2 = 0;
#endif
		child->thread.debug.dac2 = 0;
		dbcr_dac(child) &= ~(DBCR_DAC2R | DBCR_DAC2W);
	} else
		return -EINVAL;

	return 0;
}
