static JSStringRef indexRangeInTable(PlatformUIElement element, bool isRowRange)
{
    GOwnPtr<gchar> rangeString(g_strdup("{0, 0}"));

    if (!element || !ATK_IS_OBJECT(element))
        return JSStringCreateWithUTF8CString(rangeString.get());

    AtkObject* axTable = atk_object_get_parent(ATK_OBJECT(element));
    if (!axTable || !ATK_IS_TABLE(axTable))
        return JSStringCreateWithUTF8CString(rangeString.get());

    gint indexInParent = atk_object_get_index_in_parent(ATK_OBJECT(element));
    if (indexInParent == -1)
        return JSStringCreateWithUTF8CString(rangeString.get());

    int row = -1;
    int column = -1;
    row = atk_table_get_row_at_index(ATK_TABLE(axTable), indexInParent);
    column = atk_table_get_column_at_index(ATK_TABLE(axTable), indexInParent);

    if (row != -1 && column != -1) {
        int base = 0;
        int length = 0;
        if (isRowRange) {
            base = row;
            length = atk_table_get_row_extent_at(ATK_TABLE(axTable), row, column);
        } else {
            base = column;
            length = atk_table_get_column_extent_at(ATK_TABLE(axTable), row, column);
        }
        rangeString.set(g_strdup_printf("{%d, %d}", base, length));
    }

    return JSStringCreateWithUTF8CString(rangeString.get());
}
