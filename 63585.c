static int dvd_do_auth(struct cdrom_device_info *cdi, dvd_authinfo *ai)
{
	int ret;
	u_char buf[20];
	struct packet_command cgc;
	const struct cdrom_device_ops *cdo = cdi->ops;
	rpc_state_t rpc_state;

	memset(buf, 0, sizeof(buf));
	init_cdrom_command(&cgc, buf, 0, CGC_DATA_READ);

	switch (ai->type) {
	/* LU data send */
	case DVD_LU_SEND_AGID:
		cd_dbg(CD_DVD, "entering DVD_LU_SEND_AGID\n");
		cgc.quiet = 1;
		setup_report_key(&cgc, ai->lsa.agid, 0);

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		ai->lsa.agid = buf[7] >> 6;
		/* Returning data, let host change state */
		break;

	case DVD_LU_SEND_KEY1:
		cd_dbg(CD_DVD, "entering DVD_LU_SEND_KEY1\n");
		setup_report_key(&cgc, ai->lsk.agid, 2);

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		copy_key(ai->lsk.key, &buf[4]);
		/* Returning data, let host change state */
		break;

	case DVD_LU_SEND_CHALLENGE:
		cd_dbg(CD_DVD, "entering DVD_LU_SEND_CHALLENGE\n");
		setup_report_key(&cgc, ai->lsc.agid, 1);

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		copy_chal(ai->lsc.chal, &buf[4]);
		/* Returning data, let host change state */
		break;

	/* Post-auth key */
	case DVD_LU_SEND_TITLE_KEY:
		cd_dbg(CD_DVD, "entering DVD_LU_SEND_TITLE_KEY\n");
		cgc.quiet = 1;
		setup_report_key(&cgc, ai->lstk.agid, 4);
		cgc.cmd[5] = ai->lstk.lba;
		cgc.cmd[4] = ai->lstk.lba >> 8;
		cgc.cmd[3] = ai->lstk.lba >> 16;
		cgc.cmd[2] = ai->lstk.lba >> 24;

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		ai->lstk.cpm = (buf[4] >> 7) & 1;
		ai->lstk.cp_sec = (buf[4] >> 6) & 1;
		ai->lstk.cgms = (buf[4] >> 4) & 3;
		copy_key(ai->lstk.title_key, &buf[5]);
		/* Returning data, let host change state */
		break;

	case DVD_LU_SEND_ASF:
		cd_dbg(CD_DVD, "entering DVD_LU_SEND_ASF\n");
		setup_report_key(&cgc, ai->lsasf.agid, 5);
		
		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		ai->lsasf.asf = buf[7] & 1;
		break;

	/* LU data receive (LU changes state) */
	case DVD_HOST_SEND_CHALLENGE:
		cd_dbg(CD_DVD, "entering DVD_HOST_SEND_CHALLENGE\n");
		setup_send_key(&cgc, ai->hsc.agid, 1);
		buf[1] = 0xe;
		copy_chal(&buf[4], ai->hsc.chal);

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		ai->type = DVD_LU_SEND_KEY1;
		break;

	case DVD_HOST_SEND_KEY2:
		cd_dbg(CD_DVD, "entering DVD_HOST_SEND_KEY2\n");
		setup_send_key(&cgc, ai->hsk.agid, 3);
		buf[1] = 0xa;
		copy_key(&buf[4], ai->hsk.key);

		if ((ret = cdo->generic_packet(cdi, &cgc))) {
			ai->type = DVD_AUTH_FAILURE;
			return ret;
		}
		ai->type = DVD_AUTH_ESTABLISHED;
		break;

	/* Misc */
	case DVD_INVALIDATE_AGID:
		cgc.quiet = 1;
		cd_dbg(CD_DVD, "entering DVD_INVALIDATE_AGID\n");
		setup_report_key(&cgc, ai->lsa.agid, 0x3f);
		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;
		break;

	/* Get region settings */
	case DVD_LU_SEND_RPC_STATE:
		cd_dbg(CD_DVD, "entering DVD_LU_SEND_RPC_STATE\n");
		setup_report_key(&cgc, 0, 8);
		memset(&rpc_state, 0, sizeof(rpc_state_t));
		cgc.buffer = (char *) &rpc_state;

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;

		ai->lrpcs.type = rpc_state.type_code;
		ai->lrpcs.vra = rpc_state.vra;
		ai->lrpcs.ucca = rpc_state.ucca;
		ai->lrpcs.region_mask = rpc_state.region_mask;
		ai->lrpcs.rpc_scheme = rpc_state.rpc_scheme;
		break;

	/* Set region settings */
	case DVD_HOST_SEND_RPC_STATE:
		cd_dbg(CD_DVD, "entering DVD_HOST_SEND_RPC_STATE\n");
		setup_send_key(&cgc, 0, 6);
		buf[1] = 6;
		buf[4] = ai->hrpcs.pdrc;

		if ((ret = cdo->generic_packet(cdi, &cgc)))
			return ret;
		break;

	default:
		cd_dbg(CD_WARNING, "Invalid DVD key ioctl (%d)\n", ai->type);
		return -ENOTTY;
	}

	return 0;
}
