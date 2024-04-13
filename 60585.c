int main(int argc, char **argv)
{
    struct sigaction sigact = { { 0 } };
    int cfg_parsed;
    int ret = EXIT_FAILURE;

    init_dynload();

    config.filename = av_strdup("/etc/ffserver.conf");

    parse_loglevel(argc, argv, options);
    av_register_all();
    avformat_network_init();

    show_banner(argc, argv, options);

    my_program_name = argv[0];

    parse_options(NULL, argc, argv, options, NULL);

    unsetenv("http_proxy");             /* Kill the http_proxy */

    av_lfg_init(&random_state, av_get_random_seed());

    sigact.sa_handler = handle_child_exit;
    sigact.sa_flags = SA_NOCLDSTOP | SA_RESTART;
    sigaction(SIGCHLD, &sigact, 0);

    if ((cfg_parsed = ffserver_parse_ffconfig(config.filename, &config)) < 0) {
        fprintf(stderr, "Error reading configuration file '%s': %s\n",
                config.filename, av_err2str(cfg_parsed));
        goto bail;
    }

    /* open log file if needed */
    if (config.logfilename[0] != '\0') {
        if (!strcmp(config.logfilename, "-"))
            logfile = stdout;
        else
            logfile = fopen(config.logfilename, "a");
        av_log_set_callback(http_av_log);
    }

    build_file_streams();

    if (build_feed_streams() < 0) {
        http_log("Could not setup feed streams\n");
        goto bail;
    }

    compute_bandwidth();

    /* signal init */
    signal(SIGPIPE, SIG_IGN);

    if (http_server() < 0) {
        http_log("Could not start server\n");
        goto bail;
    }

    ret=EXIT_SUCCESS;

bail:
    av_freep (&config.filename);
    avformat_network_deinit();
    return ret;
}
