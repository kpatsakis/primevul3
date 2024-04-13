rdp_out_ts_glyphcache_capabilityset(STREAM s)
{
	uint16 supportlvl = GLYPH_SUPPORT_FULL;
	uint32 fragcache = 0x01000100;
	out_uint16_le(s, RDP_CAPSET_GLYPHCACHE);
	out_uint16_le(s, RDP_CAPLEN_GLYPHCACHE);

	/* GlyphCache - 10 TS_CACHE_DEFINITION structures */
	rdp_out_ts_cache_definition(s, 254, 4);
	rdp_out_ts_cache_definition(s, 254, 4);
	rdp_out_ts_cache_definition(s, 254, 8);
	rdp_out_ts_cache_definition(s, 254, 8);
	rdp_out_ts_cache_definition(s, 254, 16);
	rdp_out_ts_cache_definition(s, 254, 32);
	rdp_out_ts_cache_definition(s, 254, 64);
	rdp_out_ts_cache_definition(s, 254, 128);
	rdp_out_ts_cache_definition(s, 254, 256);
	rdp_out_ts_cache_definition(s, 64, 2048);

	out_uint32_le(s, fragcache);	/* FragCache */
	out_uint16_le(s, supportlvl);	/* GlyphSupportLevel */
	out_uint16_le(s, 0);	/* pad2octets */
}
