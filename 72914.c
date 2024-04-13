sec_out_mcs_connect_initial_pdu(STREAM s, uint32 selected_protocol)
{
	int length = 162 + 76 + 12 + 4 + (g_dpi > 0 ? 18 : 0);
	unsigned int i;
	uint32 rdpversion = RDP_40;
	uint16 capflags = RNS_UD_CS_SUPPORT_ERRINFO_PDU;
	uint16 colorsupport = RNS_UD_24BPP_SUPPORT | RNS_UD_16BPP_SUPPORT | RNS_UD_32BPP_SUPPORT;
	uint32 physwidth, physheight, desktopscale, devicescale;

	logger(Protocol, Debug, "%s()", __func__);

	if (g_rdp_version >= RDP_V5)
		rdpversion = RDP_50;

	if (g_num_channels > 0)
		length += g_num_channels * 12 + 8;

	/* Generic Conference Control (T.124) ConferenceCreateRequest */
	out_uint16_be(s, 5);
	out_uint16_be(s, 0x14);
	out_uint8(s, 0x7c);
	out_uint16_be(s, 1);

	out_uint16_be(s, (length | 0x8000));	/* remaining length */

	out_uint16_be(s, 8);	/* length? */
	out_uint16_be(s, 16);
	out_uint8(s, 0);
	out_uint16_le(s, 0xc001);
	out_uint8(s, 0);

	out_uint32_le(s, 0x61637544);	/* OEM ID: "Duca", as in Ducati. */
	out_uint16_be(s, ((length - 14) | 0x8000));	/* remaining length */

	/* Client information (TS_UD_CS_CORE) */
	out_uint16_le(s, CS_CORE);	/* type */
	out_uint16_le(s, 216 + (g_dpi > 0 ? 18 : 0));	/* length */
	out_uint32_le(s, rdpversion);	/* version */
	out_uint16_le(s, g_requested_session_width);	/* desktopWidth */
	out_uint16_le(s, g_requested_session_height);	/* desktopHeight */
	out_uint16_le(s, RNS_UD_COLOR_8BPP);	/* colorDepth */
	out_uint16_le(s, RNS_UD_SAS_DEL);	/* SASSequence */
	out_uint32_le(s, g_keylayout);	/* keyboardLayout */
	out_uint32_le(s, 2600);	/* Client build. We are now 2600 compatible :-) */

	/* Unicode name of client, padded to 32 bytes */
	out_utf16s_padded(s, g_hostname, 32, 0x00);

	out_uint32_le(s, g_keyboard_type);	/* keyboardType */
	out_uint32_le(s, g_keyboard_subtype);	/* keyboardSubtype */
	out_uint32_le(s, g_keyboard_functionkeys);	/* keyboardFunctionKey */
	out_uint8s(s, 64);	/* imeFileName */
	out_uint16_le(s, RNS_UD_COLOR_8BPP);	/* postBeta2ColorDepth (overrides colorDepth) */
	out_uint16_le(s, 1);	/* clientProductId (should be 1) */
	out_uint32_le(s, 0);	/* serialNumber (should be 0) */

	/* highColorDepth (overrides postBeta2ColorDepth). Capped at 24BPP.
	   To get 32BPP sessions, we need to set a capability flag. */
	out_uint16_le(s, MIN(g_server_depth, 24));
	if (g_server_depth == 32)
		capflags |= RNS_UD_CS_WANT_32BPP_SESSION;

	out_uint16_le(s, colorsupport);	/* supportedColorDepths */
	out_uint16_le(s, capflags);	/* earlyCapabilityFlags */
	out_uint8s(s, 64);	/* clientDigProductId */
	out_uint8(s, 0);	/* connectionType */
	out_uint8(s, 0);	/* pad */
	out_uint32_le(s, selected_protocol);	/* serverSelectedProtocol */
	if (g_dpi > 0)
	{
		/* Extended client info describing monitor geometry */
		utils_calculate_dpi_scale_factors(g_requested_session_width,
						  g_requested_session_height, g_dpi, &physwidth,
						  &physheight, &desktopscale, &devicescale);
		out_uint32_le(s, physwidth);	/* physicalwidth */
		out_uint32_le(s, physheight);	/* physicalheight */
		out_uint16_le(s, ORIENTATION_LANDSCAPE);	/* Orientation */
		out_uint32_le(s, desktopscale);	/* DesktopScaleFactor */
		out_uint32_le(s, devicescale);	/* DeviceScaleFactor */
	}

	/* Write a Client Cluster Data (TS_UD_CS_CLUSTER) */
	uint32 cluster_flags = 0;
	out_uint16_le(s, CS_CLUSTER);	/* header.type */
	out_uint16_le(s, 12);	/* length */

	cluster_flags |= SEC_CC_REDIRECTION_SUPPORTED;
	cluster_flags |= (SEC_CC_REDIRECT_VERSION_3 << 2);

	if (g_console_session || g_redirect_session_id != 0)
		cluster_flags |= SEC_CC_REDIRECT_SESSIONID_FIELD_VALID;

	out_uint32_le(s, cluster_flags);
	out_uint32(s, g_redirect_session_id);

	/* Client encryption settings (TS_UD_CS_SEC) */
	out_uint16_le(s, CS_SECURITY);	/* type */
	out_uint16_le(s, 12);	/* length */
	out_uint32_le(s, g_encryption ? 0x3 : 0);	/* encryptionMethods */
	out_uint32(s, 0);	/* extEncryptionMethods */

	/* Channel definitions (TS_UD_CS_NET) */
	logger(Protocol, Debug, "sec_out_mcs_data(), g_num_channels is %d", g_num_channels);
	if (g_num_channels > 0)
	{
		out_uint16_le(s, CS_NET);	/* type */
		out_uint16_le(s, g_num_channels * 12 + 8);	/* length */
		out_uint32_le(s, g_num_channels);	/* number of virtual channels */
		for (i = 0; i < g_num_channels; i++)
		{
			logger(Protocol, Debug, "sec_out_mcs_data(), requesting channel %s",
			       g_channels[i].name);
			out_uint8a(s, g_channels[i].name, 8);
			out_uint32_be(s, g_channels[i].flags);
		}
	}

	s_mark_end(s);
}
