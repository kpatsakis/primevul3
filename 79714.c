void HTMLKeygenElement::didAddUserAgentShadowRoot(ShadowRoot& root)
{
    DEFINE_STATIC_LOCAL(AtomicString, keygenSelectPseudoId, ("-webkit-keygen-select", AtomicString::ConstructFromLiteral));

    Vector<String> keys;
    getSupportedKeySizes(locale(), keys);

    RefPtr<HTMLSelectElement> select = HTMLSelectElement::create(document());
    select->setShadowPseudoId(keygenSelectPseudoId);
    for (size_t i = 0; i < keys.size(); ++i) {
        RefPtr<HTMLOptionElement> option = HTMLOptionElement::create(document());
        option->appendChild(Text::create(document(), keys[i]));
        select->appendChild(option);
    }

    root.appendChild(select);
}
