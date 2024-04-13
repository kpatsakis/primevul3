static void _send_back_fd(int socket, int fd)
{
	struct msghdr msg = { 0 };
	struct cmsghdr *cmsg;
	char buf[CMSG_SPACE(sizeof(fd))];
	memset(buf, '\0', sizeof(buf));

	msg.msg_iov = NULL;
	msg.msg_iovlen = 0;
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_level = SOL_SOCKET;
	cmsg->cmsg_type = SCM_RIGHTS;
	cmsg->cmsg_len = CMSG_LEN(sizeof(fd));

	memmove(CMSG_DATA(cmsg), &fd, sizeof(fd));
	msg.msg_controllen = cmsg->cmsg_len;

	if (sendmsg(socket, &msg, 0) < 0)
		error("%s: failed to send fd: %m", __func__);
}
