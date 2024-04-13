megasas_decode_evt(struct megasas_instance *instance)
{
	struct megasas_evt_detail *evt_detail = instance->evt_detail;
	union megasas_evt_class_locale class_locale;
	class_locale.word = le32_to_cpu(evt_detail->cl.word);

	if (class_locale.members.class >= MFI_EVT_CLASS_CRITICAL)
		dev_info(&instance->pdev->dev, "%d (%s/0x%04x/%s) - %s\n",
			le32_to_cpu(evt_detail->seq_num),
			format_timestamp(le32_to_cpu(evt_detail->time_stamp)),
			(class_locale.members.locale),
			format_class(class_locale.members.class),
			evt_detail->description);
}
