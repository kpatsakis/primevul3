int getch() {
	struct termios old;
	struct termios new;
	int rc;
	if (tcgetattr(0, &old) == -1) {
		return -1;
	}
	new = old;
	new.c_lflag &= ~(ICANON | ECHO);
	new.c_cc[VMIN] = 1;
	new.c_cc[VTIME] = 0;
	if (tcsetattr(0, TCSANOW, &new) == -1) {
		return -1;
	}
	rc = getchar();
	(void) tcsetattr(0, TCSANOW, &old);
	return rc;
}
