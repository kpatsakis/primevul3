static int _receive_fd(int socket)
{
	struct msghdr msg = {0};
	struct cmsghdr *cmsg;
	int fd;
	msg.msg_iov = NULL;
	msg.msg_iovlen = 0;
	char c_buffer[256];
	msg.msg_control = c_buffer;
	msg.msg_controllen = sizeof(c_buffer);

	if (recvmsg(socket, &msg, 0) < 0) {
		error("%s: failed to receive fd: %m", __func__);
		return -1;
	}

	cmsg = CMSG_FIRSTHDR(&msg);
	memmove(&fd, CMSG_DATA(cmsg), sizeof(fd));
	return fd;
}
