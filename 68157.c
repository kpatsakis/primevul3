ptaaGetPta(PTAA    *ptaa,
           l_int32  index,
           l_int32  accessflag)
{
    PROCNAME("ptaaGetPta");

    if (!ptaa)
        return (PTA *)ERROR_PTR("ptaa not defined", procName, NULL);
    if (index < 0 || index >= ptaa->n)
        return (PTA *)ERROR_PTR("index not valid", procName, NULL);

    if (accessflag == L_COPY)
        return ptaCopy(ptaa->pta[index]);
    else if (accessflag == L_CLONE)
        return ptaClone(ptaa->pta[index]);
    else
        return (PTA *)ERROR_PTR("invalid accessflag", procName, NULL);
}
