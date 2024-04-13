static ssize_t ipmi_params_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	struct smi_info *smi_info = dev_get_drvdata(dev);

	return snprintf(buf, 200,
			"%s,%s,0x%lx,rsp=%d,rsi=%d,rsh=%d,irq=%d,ipmb=%d\n",
			si_to_str[smi_info->io.si_type],
			addr_space_to_str[smi_info->io.addr_type],
			smi_info->io.addr_data,
			smi_info->io.regspacing,
			smi_info->io.regsize,
			smi_info->io.regshift,
			smi_info->io.irq,
			smi_info->io.slave_addr);
}
