rdp_out_ts_sound_capabilityset(STREAM s)
{
	uint16 soundflags = SOUND_BEEPS_FLAG;

	out_uint16_le(s, RDP_CAPSET_SOUND);
	out_uint16_le(s, RDP_CAPLEN_SOUND);

	out_uint16_le(s, soundflags);	/* soundFlags */
	out_uint16_le(s, 0);	/* pad2OctetsA */
}
