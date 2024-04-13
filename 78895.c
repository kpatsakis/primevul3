String Element::innerText()
{
    document()->updateLayoutIgnorePendingStylesheets();

    if (!renderer())
        return textContent(true);

    return plainText(rangeOfContents(const_cast<Element*>(this)).get());
}
