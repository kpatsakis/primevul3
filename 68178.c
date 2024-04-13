selGetElement(SEL      *sel,
              l_int32   row,
              l_int32   col,
              l_int32  *ptype)
{
    PROCNAME("selGetElement");

    if (!ptype)
        return ERROR_INT("&type not defined", procName, 1);
    *ptype = SEL_DONT_CARE;
    if (!sel)
        return ERROR_INT("sel not defined", procName, 1);
    if (row < 0 || row >= sel->sy)
        return ERROR_INT("sel row out of bounds", procName, 1);
    if (col < 0 || col >= sel->sx)
        return ERROR_INT("sel col out of bounds", procName, 1);

    *ptype = sel->data[row][col];
    return 0;
}
