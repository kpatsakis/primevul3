static int _store_item_copy_data(int comm, item *old_it, item *new_it, item *add_it) {
    if (comm == NREAD_APPEND) {
        if (new_it->it_flags & ITEM_CHUNKED) {
            if (_store_item_copy_chunks(new_it, old_it, old_it->nbytes - 2) == -1 ||
                _store_item_copy_chunks(new_it, add_it, add_it->nbytes) == -1) {
                return -1;
            }
        } else {
            memcpy(ITEM_data(new_it), ITEM_data(old_it), old_it->nbytes);
            memcpy(ITEM_data(new_it) + old_it->nbytes - 2 /* CRLF */, ITEM_data(add_it), add_it->nbytes);
        }
    } else {
        /* NREAD_PREPEND */
        if (new_it->it_flags & ITEM_CHUNKED) {
            if (_store_item_copy_chunks(new_it, add_it, add_it->nbytes - 2) == -1 ||
                _store_item_copy_chunks(new_it, old_it, old_it->nbytes) == -1) {
                return -1;
            }
        } else {
            memcpy(ITEM_data(new_it), ITEM_data(add_it), add_it->nbytes);
            memcpy(ITEM_data(new_it) + add_it->nbytes - 2 /* CRLF */, ITEM_data(old_it), old_it->nbytes);
        }
    }
    return 0;
}
