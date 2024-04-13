void XSLStyleSheet::checkLoaded()
{
    if (isLoading())
        return;
    if (XSLStyleSheet* styleSheet = parentStyleSheet())
        styleSheet->checkLoaded();
    if (ownerNode())
        ownerNode()->sheetLoaded();
}
