bool HTMLKeygenElement::appendFormData(FormDataList& encoding, bool)
{
    const AtomicString& keyType = fastGetAttribute(keytypeAttr);
    if (!keyType.isNull() && !equalIgnoringCase(keyType, "rsa"))
        return false;
    String value = signedPublicKeyAndChallengeString(shadowSelect()->selectedIndex(), fastGetAttribute(challengeAttr), document().baseURL());
    if (value.isNull())
        return false;
    encoding.appendData(name(), value.utf8());
    return true;
}
