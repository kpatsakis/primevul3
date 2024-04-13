void CSSStyleSheet::styleSheetChanged()
{
    StyleBase* root = this;
    while (StyleBase* parent = root->parent())
        root = parent;
    Document* documentToUpdate = root->isCSSStyleSheet() ? static_cast<CSSStyleSheet*>(root)->doc() : 0;
    
    /* FIXME: We don't need to do everything updateStyleSelector does,
     * basically we just need to recreate the document's selector with the
     * already existing style sheets.
     */
    if (documentToUpdate)
        documentToUpdate->updateStyleSelector();
}
