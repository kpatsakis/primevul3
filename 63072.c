static void add_bin_header(conn *c, uint16_t err, uint8_t hdr_len, uint16_t key_len, uint32_t body_len) {
    protocol_binary_response_header* header;

    assert(c);

    c->msgcurr = 0;
    c->msgused = 0;
    c->iovused = 0;
    if (add_msghdr(c) != 0) {
        /* This should never run out of memory because iov and msg lists
         * have minimum sizes big enough to hold an error response.
         */
        out_of_memory(c, "SERVER_ERROR out of memory adding binary header");
        return;
    }

    header = (protocol_binary_response_header *)c->wbuf;

    header->response.magic = (uint8_t)PROTOCOL_BINARY_RES;
    header->response.opcode = c->binary_header.request.opcode;
    header->response.keylen = (uint16_t)htons(key_len);

    header->response.extlen = (uint8_t)hdr_len;
    header->response.datatype = (uint8_t)PROTOCOL_BINARY_RAW_BYTES;
    header->response.status = (uint16_t)htons(err);

    header->response.bodylen = htonl(body_len);
    header->response.opaque = c->opaque;
    header->response.cas = htonll(c->cas);

    if (settings.verbose > 1) {
        int ii;
        fprintf(stderr, ">%d Writing bin response:", c->sfd);
        for (ii = 0; ii < sizeof(header->bytes); ++ii) {
            if (ii % 4 == 0) {
                fprintf(stderr, "\n>%d  ", c->sfd);
            }
            fprintf(stderr, " 0x%02x", header->bytes[ii]);
        }
        fprintf(stderr, "\n");
    }

    add_iov(c, c->wbuf, sizeof(header->response));
}
