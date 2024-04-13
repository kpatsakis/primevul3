ApplyBlockElementCommand::ApplyBlockElementCommand(Document& document, const QualifiedName& tagName)
    : CompositeEditCommand(document)
    , m_tagName(tagName)
{
}
