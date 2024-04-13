u32 rds_tcp_snd_una(struct rds_tcp_connection *tc)
{
	return tcp_sk(tc->t_sock->sk)->snd_una;
}
