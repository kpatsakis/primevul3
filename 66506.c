av_cold void ff_init_scantable_permutation(uint8_t *idct_permutation,
                                           enum idct_permutation_type perm_type)
{
    int i;

    if (ARCH_X86)
        if (ff_init_scantable_permutation_x86(idct_permutation,
                                              perm_type))
            return;

    switch (perm_type) {
    case FF_IDCT_PERM_NONE:
        for (i = 0; i < 64; i++)
            idct_permutation[i] = i;
        break;
    case FF_IDCT_PERM_LIBMPEG2:
        for (i = 0; i < 64; i++)
            idct_permutation[i] = (i & 0x38) | ((i & 6) >> 1) | ((i & 1) << 2);
        break;
    case FF_IDCT_PERM_TRANSPOSE:
        for (i = 0; i < 64; i++)
            idct_permutation[i] = ((i & 7) << 3) | (i >> 3);
        break;
    case FF_IDCT_PERM_PARTTRANS:
        for (i = 0; i < 64; i++)
            idct_permutation[i] = (i & 0x24) | ((i & 3) << 3) | ((i >> 3) & 3);
        break;
    default:
        av_log(NULL, AV_LOG_ERROR,
               "Internal error, IDCT permutation not set\n");
    }
}
