selCreate(l_int32      height,
          l_int32      width,
          const char  *name)
{
SEL  *sel;

    PROCNAME("selCreate");

    if ((sel = (SEL *)LEPT_CALLOC(1, sizeof(SEL))) == NULL)
        return (SEL *)ERROR_PTR("sel not made", procName, NULL);
    if (name)
        sel->name = stringNew(name);
    sel->sy = height;
    sel->sx = width;
    if ((sel->data = create2dIntArray(height, width)) == NULL) {
        LEPT_FREE(sel->name);
        LEPT_FREE(sel);
        return (SEL *)ERROR_PTR("data not allocated", procName, NULL);
    }

    return sel;
}
