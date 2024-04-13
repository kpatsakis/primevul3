static char *write_metadata (WavpackMetadata *wpmd, char *outdata)
{
    unsigned char id = wpmd->id, wordlen [3];

    wordlen [0] = (wpmd->byte_length + 1) >> 1;
    wordlen [1] = (wpmd->byte_length + 1) >> 9;
    wordlen [2] = (wpmd->byte_length + 1) >> 17;

    if (wpmd->byte_length & 1)
        id |= ID_ODD_SIZE;

    if (wordlen [1] || wordlen [2])
        id |= ID_LARGE;

    *outdata++ = id;
    *outdata++ = wordlen [0];

    if (id & ID_LARGE) {
        *outdata++ = wordlen [1];
        *outdata++ = wordlen [2];
    }

    if (wpmd->data && wpmd->byte_length) {
        memcpy (outdata, wpmd->data, wpmd->byte_length);
        outdata += wpmd->byte_length;

        if (wpmd->byte_length & 1)
            *outdata++ = 0;
    }

    return outdata;
}
