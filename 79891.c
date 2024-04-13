ScriptLoaderClient* ScriptLoader::client() const
{
    if (isHTMLScriptLoader(m_element))
        return toHTMLScriptElement(m_element);

    if (isSVGScriptLoader(m_element))
        return toSVGScriptElement(m_element);

    ASSERT_NOT_REACHED();
    return 0;
}
