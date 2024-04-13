bool EditorClientBlackBerry::shouldInsertNode(Node* node, Range* range, EditorInsertAction insertAction)
{
    if (m_webPagePrivate->m_dumpRenderTree)
        return m_webPagePrivate->m_dumpRenderTree->shouldInsertNode(node, range, static_cast<int>(insertAction));
    return true;
}
