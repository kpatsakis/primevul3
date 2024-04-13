static void mpeg4_load_default_matrices(MpegEncContext *s)
{
    int i, v;

    /* load default matrices */
    for (i = 0; i < 64; i++) {
        int j = s->idsp.idct_permutation[i];
        v = ff_mpeg4_default_intra_matrix[i];
        s->intra_matrix[j]        = v;
        s->chroma_intra_matrix[j] = v;

        v = ff_mpeg4_default_non_intra_matrix[i];
        s->inter_matrix[j]        = v;
        s->chroma_inter_matrix[j] = v;
    }
}
