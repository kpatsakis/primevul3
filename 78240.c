bool HTMLFormElement::formWouldHaveSecureSubmission(const String& url)
{
    return document().completeURL(url).protocolIs("https");
}
