format_class(int8_t class)
{
	static char buffer[6];

	switch (class) {
	case MFI_EVT_CLASS_DEBUG:
		return "debug";
	case MFI_EVT_CLASS_PROGRESS:
		return "progress";
	case MFI_EVT_CLASS_INFO:
		return "info";
	case MFI_EVT_CLASS_WARNING:
		return "WARN";
	case MFI_EVT_CLASS_CRITICAL:
		return "CRIT";
	case MFI_EVT_CLASS_FATAL:
		return "FATAL";
	case MFI_EVT_CLASS_DEAD:
		return "DEAD";
	default:
		snprintf(buffer, sizeof(buffer), "%d", class);
		return buffer;
	}
}
