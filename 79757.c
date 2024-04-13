const AtomicString& HTMLTextAreaElement::formControlType() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, textarea, ("textarea", AtomicString::ConstructFromLiteral));
    return textarea;
}
