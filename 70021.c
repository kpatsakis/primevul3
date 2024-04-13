static void rds_conn_peer_gen_update(struct rds_connection *conn,
				     u32 peer_gen_num)
{
	int i;
	struct rds_message *rm, *tmp;
	unsigned long flags;

	WARN_ON(conn->c_trans->t_type != RDS_TRANS_TCP);
	if (peer_gen_num != 0) {
		if (conn->c_peer_gen_num != 0 &&
		    peer_gen_num != conn->c_peer_gen_num) {
			for (i = 0; i < RDS_MPATH_WORKERS; i++) {
				struct rds_conn_path *cp;

				cp = &conn->c_path[i];
				spin_lock_irqsave(&cp->cp_lock, flags);
				cp->cp_next_tx_seq = 1;
				cp->cp_next_rx_seq = 0;
				list_for_each_entry_safe(rm, tmp,
							 &cp->cp_retrans,
							 m_conn_item) {
					set_bit(RDS_MSG_FLUSH, &rm->m_flags);
				}
				spin_unlock_irqrestore(&cp->cp_lock, flags);
			}
		}
		conn->c_peer_gen_num = peer_gen_num;
	}
}
