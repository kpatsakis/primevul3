int copy_metadata (WavpackMetadata *wpmd, unsigned char *buffer_start, unsigned char *buffer_end)
{
    uint32_t mdsize = wpmd->byte_length + (wpmd->byte_length & 1);
    WavpackHeader *wphdr = (WavpackHeader *) buffer_start;

    mdsize += (wpmd->byte_length > 510) ? 4 : 2;
    buffer_start += wphdr->ckSize + 8;

    if (buffer_start + mdsize >= buffer_end)
        return FALSE;

    buffer_start [0] = wpmd->id | (wpmd->byte_length & 1 ? ID_ODD_SIZE : 0);
    buffer_start [1] = (wpmd->byte_length + 1) >> 1;

    if (wpmd->byte_length > 510) {
        buffer_start [0] |= ID_LARGE;
        buffer_start [2] = (wpmd->byte_length + 1) >> 9;
        buffer_start [3] = (wpmd->byte_length + 1) >> 17;
    }

    if (wpmd->data && wpmd->byte_length) {
        memcpy (buffer_start + (wpmd->byte_length > 510 ? 4 : 2), wpmd->data, wpmd->byte_length);

        if (wpmd->byte_length & 1)          // if size is odd, make sure pad byte is a zero
            buffer_start [mdsize - 1] = 0;
    }

    wphdr->ckSize += mdsize;
    return TRUE;
}
