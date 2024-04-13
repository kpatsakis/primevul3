const AtomicString& HTMLKeygenElement::formControlType() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, keygen, ("keygen", AtomicString::ConstructFromLiteral));
    return keygen;
}
