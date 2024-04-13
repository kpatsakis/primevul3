static bool deadline_exceeded(const struct timespec *d)
{
    struct timespec now;

    if (d->tv_sec <= 0) {
        /* No deadline configured */
        return false;
    }

    errno = 0;
    if (clock_gettime(CLOCK_MONOTONIC, &now) == -1) {
        syslog(LOG_ERR, "clock_gettime (%d %m): error reading clock", errno);
        return false;
    }

    return now.tv_sec > d->tv_sec ||
           (now.tv_sec == d->tv_sec && now.tv_nsec > d->tv_nsec);
}
