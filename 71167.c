u32 rds_tcp_write_seq(struct rds_tcp_connection *tc)
{
	/* seq# of the last byte of data in tcp send buffer */
	return tcp_sk(tc->t_sock->sk)->write_seq;
}
