pump_data_until_message(LoadContext *lc,Image *image) /* ddjvu_context_t *context, type ddjvu_document_type_t */
{
        size_t blocksize = BLOCKSIZE;
        unsigned char data[BLOCKSIZE];
        size_t size;
        ddjvu_message_t *message;

        /* i might check for a condition! */
        size=0;
        while (!(message = ddjvu_message_peek(lc->context))
               && (size = (size_t) ReadBlob(image,(size_t) blocksize,data)) == blocksize) {
                ddjvu_stream_write(lc->document, lc->streamid, (char *) data, size);
        }
        if (message)
                return message;
        if (size)
                ddjvu_stream_write(lc->document, lc->streamid, (char *) data, size);
        ddjvu_stream_close(lc->document, lc->streamid, 0);
        return NULL;
}
