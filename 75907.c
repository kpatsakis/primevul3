  scoped_refptr<Extension> LoadAndExpectSuccess(DictionaryValue* manifest,
                                                const std::string& name) {
    std::string error;
    scoped_refptr<Extension> extension = LoadExtension(manifest, &error);
    EXPECT_TRUE(extension) << "Unexpected success for " << name;
    EXPECT_EQ("", error) << "Unexpected no error for " << name;
    return extension;
  }
