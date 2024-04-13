getCompositeParameters(l_int32   size,
                       l_int32  *psize1,
                       l_int32  *psize2,
                       char    **pnameh1,
                       char    **pnameh2,
                       char    **pnamev1,
                       char    **pnamev2)
{
l_int32  index;

    PROCNAME("selaGetSelnames");

    if (psize1) *psize1 = 0;
    if (psize2) *psize2 = 0;
    if (pnameh1) *pnameh1 = NULL;
    if (pnameh2) *pnameh2 = NULL;
    if (pnamev1) *pnamev1 = NULL;
    if (pnamev2) *pnamev2 = NULL;
    if (size < 2 || size > 63)
        return ERROR_INT("valid size range is {2 ... 63}", procName, 1);
    index = size - 2;
    if (psize1)
        *psize1 = comp_parameter_map[index].size1;
    if (psize2)
        *psize2 = comp_parameter_map[index].size2;
    if (pnameh1)
        *pnameh1 = stringNew(comp_parameter_map[index].selnameh1);
    if (pnameh2)
        *pnameh2 = stringNew(comp_parameter_map[index].selnameh2);
    if (pnamev1)
        *pnamev1 = stringNew(comp_parameter_map[index].selnamev1);
    if (pnamev2)
        *pnamev2 = stringNew(comp_parameter_map[index].selnamev2);
    return 0;
}
