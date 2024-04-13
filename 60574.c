static void handle_child_exit(int sig)
{
    pid_t pid;
    int status;
    time_t uptime;

    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        FFServerStream *feed;

        for (feed = config.first_feed; feed; feed = feed->next) {
            if (feed->pid != pid)
                continue;

            uptime = time(0) - feed->pid_start;
            feed->pid = 0;
            fprintf(stderr,
                    "%s: Pid %"PRId64" exited with status %d after %"PRId64" "
                        "seconds\n",
                    feed->filename, (int64_t) pid, status, (int64_t)uptime);

            if (uptime < 30)
                /* Turn off any more restarts */
                ffserver_free_child_args(&feed->child_argv);
        }
    }

    need_to_start_children = 1;
}
