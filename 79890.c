void ScriptLoader::childrenChanged()
{
    if (!m_parserInserted && m_element->inDocument())
        prepareScript(); // FIXME: Provide a real starting line number here.
}
