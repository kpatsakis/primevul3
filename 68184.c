selSetElement(SEL     *sel,
              l_int32  row,
              l_int32  col,
              l_int32  type)
{
    PROCNAME("selSetElement");

    if (!sel)
        return ERROR_INT("sel not defined", procName, 1);
    if (type != SEL_HIT && type != SEL_MISS && type != SEL_DONT_CARE)
        return ERROR_INT("invalid sel element type", procName, 1);
    if (row < 0 || row >= sel->sy)
        return ERROR_INT("sel row out of bounds", procName, 1);
    if (col < 0 || col >= sel->sx)
        return ERROR_INT("sel col out of bounds", procName, 1);

    sel->data[row][col] = type;
    return 0;
}
