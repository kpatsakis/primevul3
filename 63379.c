static int write_metadata_block (WavpackContext *wpc)
{
    char *block_buff, *block_ptr;
    WavpackHeader *wphdr;

    if (wpc->metacount) {
        int metacount = wpc->metacount, block_size = sizeof (WavpackHeader);
        WavpackMetadata *wpmdp = wpc->metadata;

        while (metacount--) {
            block_size += wpmdp->byte_length + (wpmdp->byte_length & 1);
            block_size += (wpmdp->byte_length > 510) ? 4 : 2;
            wpmdp++;
        }

        wphdr = (WavpackHeader *) (block_buff = malloc (block_size + 6));

        CLEAR (*wphdr);
        memcpy (wphdr->ckID, "wvpk", 4);
        SET_TOTAL_SAMPLES (*wphdr, wpc->total_samples);
        wphdr->version = wpc->stream_version;
        wphdr->ckSize = block_size - 8;
        wphdr->block_samples = 0;

        block_ptr = (char *)(wphdr + 1);

        wpmdp = wpc->metadata;

        while (wpc->metacount) {
            block_ptr = write_metadata (wpmdp, block_ptr);
            wpc->metabytes -= wpmdp->byte_length;
            free_metadata (wpmdp++);
            wpc->metacount--;
        }

        free (wpc->metadata);
        wpc->metadata = NULL;
        block_add_checksum ((unsigned char *) block_buff, (unsigned char *) block_buff + (block_size += 6), 4);
        WavpackNativeToLittleEndian ((WavpackHeader *) block_buff, WavpackHeaderFormat);

        if (!wpc->blockout (wpc->wv_out, block_buff, block_size)) {
            free (block_buff);
            strcpy (wpc->error_message, "can't write WavPack data, disk probably full!");
            return FALSE;
        }

        free (block_buff);
    }

    return TRUE;
}
