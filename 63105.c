static void process_bin_flush(conn *c) {
    time_t exptime = 0;
    protocol_binary_request_flush* req = binary_get_request(c);
    rel_time_t new_oldest = 0;

    if (!settings.flush_enabled) {
      write_bin_error(c, PROTOCOL_BINARY_RESPONSE_AUTH_ERROR, NULL, 0);
      return;
    }

    if (c->binary_header.request.extlen == sizeof(req->message.body)) {
        exptime = ntohl(req->message.body.expiration);
    }

    if (exptime > 0) {
        new_oldest = realtime(exptime);
    } else {
        new_oldest = current_time;
    }
    if (settings.use_cas) {
        settings.oldest_live = new_oldest - 1;
        if (settings.oldest_live <= current_time)
            settings.oldest_cas = get_cas_id();
    } else {
        settings.oldest_live = new_oldest;
    }

    pthread_mutex_lock(&c->thread->stats.mutex);
    c->thread->stats.flush_cmds++;
    pthread_mutex_unlock(&c->thread->stats.mutex);

    write_bin_response(c, NULL, 0, 0, 0);
}
