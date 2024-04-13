create2dIntArray(l_int32  sy,
                 l_int32  sx)
{
l_int32    i, j, success;
l_int32  **array;

    PROCNAME("create2dIntArray");

    if ((array = (l_int32 **)LEPT_CALLOC(sy, sizeof(l_int32 *))) == NULL)
        return (l_int32 **)ERROR_PTR("ptr array not made", procName, NULL);

    success = TRUE;
    for (i = 0; i < sy; i++) {
        if ((array[i] = (l_int32 *)LEPT_CALLOC(sx, sizeof(l_int32))) == NULL) {
            success = FALSE;
            break;
        }
    }
    if (success) return array;

        /* Cleanup after error */
    for (j = 0; j < i; j++)
        LEPT_FREE(array[j]);
    LEPT_FREE(array);
    return (l_int32 **)ERROR_PTR("array not made", procName, NULL);
}
