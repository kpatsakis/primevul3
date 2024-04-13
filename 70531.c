static void exit_handler(int signo)
{
	(void)signo;
	running = 0;
}
