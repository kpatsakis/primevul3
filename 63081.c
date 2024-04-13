static void complete_update_bin(conn *c) {
    protocol_binary_response_status eno = PROTOCOL_BINARY_RESPONSE_EINVAL;
    enum store_item_type ret = NOT_STORED;
    assert(c != NULL);

    item *it = c->item;

    pthread_mutex_lock(&c->thread->stats.mutex);
    c->thread->stats.slab_stats[ITEM_clsid(it)].set_cmds++;
    pthread_mutex_unlock(&c->thread->stats.mutex);

    /* We don't actually receive the trailing two characters in the bin
     * protocol, so we're going to just set them here */
    if ((it->it_flags & ITEM_CHUNKED) == 0) {
        *(ITEM_data(it) + it->nbytes - 2) = '\r';
        *(ITEM_data(it) + it->nbytes - 1) = '\n';
    } else {
        assert(c->ritem);
        item_chunk *ch = (item_chunk *) c->ritem;
        if (ch->size == ch->used)
            ch = ch->next;
        assert(ch->size - ch->used >= 2);
        ch->data[ch->used + 1] = '\r';
        ch->data[ch->used + 2] = '\n';
        ch->used += 2;
    }

    ret = store_item(it, c->cmd, c);

#ifdef ENABLE_DTRACE
    uint64_t cas = ITEM_get_cas(it);
    switch (c->cmd) {
    case NREAD_ADD:
        MEMCACHED_COMMAND_ADD(c->sfd, ITEM_key(it), it->nkey,
                              (ret == STORED) ? it->nbytes : -1, cas);
        break;
    case NREAD_REPLACE:
        MEMCACHED_COMMAND_REPLACE(c->sfd, ITEM_key(it), it->nkey,
                                  (ret == STORED) ? it->nbytes : -1, cas);
        break;
    case NREAD_APPEND:
        MEMCACHED_COMMAND_APPEND(c->sfd, ITEM_key(it), it->nkey,
                                 (ret == STORED) ? it->nbytes : -1, cas);
        break;
    case NREAD_PREPEND:
        MEMCACHED_COMMAND_PREPEND(c->sfd, ITEM_key(it), it->nkey,
                                 (ret == STORED) ? it->nbytes : -1, cas);
        break;
    case NREAD_SET:
        MEMCACHED_COMMAND_SET(c->sfd, ITEM_key(it), it->nkey,
                              (ret == STORED) ? it->nbytes : -1, cas);
        break;
    }
#endif

    switch (ret) {
    case STORED:
        /* Stored */
        write_bin_response(c, NULL, 0, 0, 0);
        break;
    case EXISTS:
        write_bin_error(c, PROTOCOL_BINARY_RESPONSE_KEY_EEXISTS, NULL, 0);
        break;
    case NOT_FOUND:
        write_bin_error(c, PROTOCOL_BINARY_RESPONSE_KEY_ENOENT, NULL, 0);
        break;
    case NOT_STORED:
    case TOO_LARGE:
    case NO_MEMORY:
        if (c->cmd == NREAD_ADD) {
            eno = PROTOCOL_BINARY_RESPONSE_KEY_EEXISTS;
        } else if(c->cmd == NREAD_REPLACE) {
            eno = PROTOCOL_BINARY_RESPONSE_KEY_ENOENT;
        } else {
            eno = PROTOCOL_BINARY_RESPONSE_NOT_STORED;
        }
        write_bin_error(c, eno, NULL, 0);
    }

    item_remove(c->item);       /* release the c->item reference */
    c->item = 0;
}
