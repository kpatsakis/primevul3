const AtomicString& HTMLButtonElement::formControlType() const
{
    switch (m_type) {
        case SUBMIT: {
            DEFINE_STATIC_LOCAL(const AtomicString, submit, ("submit", AtomicString::ConstructFromLiteral));
            return submit;
        }
        case BUTTON: {
            DEFINE_STATIC_LOCAL(const AtomicString, button, ("button", AtomicString::ConstructFromLiteral));
            return button;
        }
        case RESET: {
            DEFINE_STATIC_LOCAL(const AtomicString, reset, ("reset", AtomicString::ConstructFromLiteral));
            return reset;
        }
    }

    ASSERT_NOT_REACHED();
    return emptyAtom;
}
