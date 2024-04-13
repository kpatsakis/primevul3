static void uncurl_attach_net(struct uncurl_conn *ucc)
{
	ucc->ctx = ucc->net;
	ucc->read = net_read;
	ucc->write = net_write;
}
