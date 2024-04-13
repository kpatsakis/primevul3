void CSSStyleSheet::deleteRule(unsigned index, ExceptionCode& ec)
{
    if (index >= length()) {
        ec = INDEX_SIZE_ERR;
        return;
    }

    ec = 0;
    remove(index);
    styleSheetChanged();
}
