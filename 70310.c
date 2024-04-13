static void session_close(struct session *session)
{
	if (session->have_thread)
	{
		pcap_breakloop(session->fp);

#ifdef _WIN32
		SetEvent(pcap_getevent(session->fp));

		WaitForSingleObject(session->thread, INFINITE);

		CloseHandle(session->thread);
		session->have_thread = 0;
		session->thread = INVALID_HANDLE_VALUE;
#else
		pthread_kill(session->thread, SIGUSR1);

		pthread_join(session->thread, NULL);
		session->have_thread = 0;
		memset(&session->thread, 0, sizeof(session->thread));
#endif
	}

	if (session->sockdata != INVALID_SOCKET)
	{
		sock_close(session->sockdata, NULL, 0);
		session->sockdata = INVALID_SOCKET;
	}

	if (session->fp)
	{
		pcap_close(session->fp);
 		session->fp = NULL;
 	}
 }
