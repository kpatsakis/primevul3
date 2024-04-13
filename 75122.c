void GetIdentityFromURL(const GURL& url,
                        std::wstring* username,
                        std::wstring* password) {
  UnescapeRule::Type flags = UnescapeRule::SPACES;
  *username = UTF16ToWideHack(UnescapeAndDecodeUTF8URLComponent(url.username(),
                                                                flags, NULL));
  *password = UTF16ToWideHack(UnescapeAndDecodeUTF8URLComponent(url.password(),
                                                                flags, NULL));
}
