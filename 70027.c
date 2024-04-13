static void rds_recv_hs_exthdrs(struct rds_header *hdr,
				struct rds_connection *conn)
{
	unsigned int pos = 0, type, len;
	union {
		struct rds_ext_header_version version;
		u16 rds_npaths;
		u32 rds_gen_num;
	} buffer;
	u32 new_peer_gen_num = 0;

	while (1) {
		len = sizeof(buffer);
		type = rds_message_next_extension(hdr, &pos, &buffer, &len);
		if (type == RDS_EXTHDR_NONE)
			break;
		/* Process extension header here */
		switch (type) {
		case RDS_EXTHDR_NPATHS:
			conn->c_npaths = min_t(int, RDS_MPATH_WORKERS,
					       be16_to_cpu(buffer.rds_npaths));
			break;
		case RDS_EXTHDR_GEN_NUM:
			new_peer_gen_num = be32_to_cpu(buffer.rds_gen_num);
			break;
		default:
			pr_warn_ratelimited("ignoring unknown exthdr type "
					     "0x%x\n", type);
		}
	}
	/* if RDS_EXTHDR_NPATHS was not found, default to a single-path */
	conn->c_npaths = max_t(int, conn->c_npaths, 1);
	conn->c_ping_triggered = 0;
	rds_conn_peer_gen_update(conn, new_peer_gen_num);
}
