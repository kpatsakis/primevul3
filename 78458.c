    void registerMockedChromeURLLoad(const std::string& fileName)
    {
        URLTestHelpers::registerMockedURLFromBaseURL(WebString::fromUTF8(m_chromeURL.c_str()), WebString::fromUTF8(fileName.c_str()));
    }
