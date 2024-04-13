void CSSStyleSheet::addSubresourceStyleURLs(ListHashSet<KURL>& urls)
{
    Deque<CSSStyleSheet*> styleSheetQueue;
    styleSheetQueue.append(this);

    while (!styleSheetQueue.isEmpty()) {
        CSSStyleSheet* styleSheet = styleSheetQueue.takeFirst();

        for (unsigned i = 0; i < styleSheet->length(); ++i) {
            StyleBase* styleBase = styleSheet->item(i);
            if (!styleBase->isRule())
                continue;
            
            CSSRule* rule = static_cast<CSSRule*>(styleBase);
            if (rule->isImportRule()) {
                if (CSSStyleSheet* ruleStyleSheet = static_cast<CSSImportRule*>(rule)->styleSheet())
                    styleSheetQueue.append(ruleStyleSheet);
            }
            rule->addSubresourceStyleURLs(urls);
        }
    }
}
