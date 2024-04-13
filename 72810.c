process_system_pointer_pdu(STREAM s)
{
	uint32 system_pointer_type;
	logger(Protocol, Debug, "%s()", __func__);

	in_uint32_le(s, system_pointer_type);

	set_system_pointer(system_pointer_type);
}
