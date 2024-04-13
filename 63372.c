static int add_to_metadata (WavpackContext *wpc, void *data, uint32_t bcount, unsigned char id)
{
    WavpackMetadata *mdp;
    unsigned char *src = data;

    while (bcount) {
        if (wpc->metacount) {
            uint32_t bc = bcount;

            mdp = wpc->metadata + wpc->metacount - 1;

            if (mdp->id == id) {
                if (wpc->metabytes + bcount > 1000000)
                    bc = 1000000 - wpc->metabytes;

                mdp->data = realloc (mdp->data, mdp->byte_length + bc);
                memcpy ((char *) mdp->data + mdp->byte_length, src, bc);
                mdp->byte_length += bc;
                wpc->metabytes += bc;
                bcount -= bc;
                src += bc;

                if (wpc->metabytes >= 1000000 && !write_metadata_block (wpc))
                    return FALSE;
            }
        }

        if (bcount) {
            wpc->metadata = realloc (wpc->metadata, (wpc->metacount + 1) * sizeof (WavpackMetadata));
            mdp = wpc->metadata + wpc->metacount++;
            mdp->byte_length = 0;
            mdp->data = NULL;
            mdp->id = id;
        }
    }

    return TRUE;
}
