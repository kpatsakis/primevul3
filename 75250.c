bool CSSStyleSheet::isLoading()
{
    unsigned len = length();
    for (unsigned i = 0; i < len; ++i) {
        StyleBase* rule = item(i);
        if (rule->isImportRule() && static_cast<CSSImportRule*>(rule)->isLoading())
            return true;
    }
    return false;
}
