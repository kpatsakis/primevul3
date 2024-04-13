EXPORTED void fatal(const char *s, int code)
{
    static int recurse_code = 0;

    if (recurse_code) {
        /* We were called recursively. Just give up */
        proc_cleanup();
        snmp_increment(ACTIVE_CONNECTIONS, -1);
        exit(recurse_code);
    }
    recurse_code = code;
    if (imapd_out) {
        prot_printf(imapd_out, "* BYE Fatal error: %s\r\n", s);
        prot_flush(imapd_out);
    }
    if (stages.count) {
        /* Cleanup the stage(s) */
        struct appendstage *curstage;
        while ((curstage = ptrarray_pop(&stages))) {
            if (curstage->f != NULL) fclose(curstage->f);
            append_removestage(curstage->stage);
            strarray_fini(&curstage->flags);
            freeentryatts(curstage->annotations);
            free(curstage);
        }
        ptrarray_fini(&stages);
    }

    syslog(LOG_ERR, "Fatal error: %s", s);
    shut_down(code);
}
