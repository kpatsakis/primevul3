static void sig_handler(const int sig) {
    printf("Signal handled: %s.\n", strsignal(sig));
    exit(EXIT_SUCCESS);
}
