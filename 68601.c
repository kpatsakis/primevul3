static int smtp_getsock(struct connectdata *conn, curl_socket_t *socks,
                        int numsocks)
{
  return Curl_pp_getsock(&conn->proto.smtpc.pp, socks, numsocks);
}
