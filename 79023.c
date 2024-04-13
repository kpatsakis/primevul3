bool Element::webkitMatchesSelector(const String& selector, ExceptionCode& ec)
{
    if (selector.isEmpty()) {
        ec = SYNTAX_ERR;
        return false;
    }

    SelectorQuery* selectorQuery = document()->selectorQueryCache()->add(selector, document(), ec);
    if (!selectorQuery)
        return false;
    return selectorQuery->matches(this);
}
