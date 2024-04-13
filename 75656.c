static void PrepareStringForFileOps(const FilePath& text,
                                    FilePath::StringType* output) {
#if defined(OS_WIN)
  TrimWhitespace(text.value(), TRIM_ALL, output);
  replace(output->begin(), output->end(), '/', '\\');
#else
  TrimWhitespaceUTF8(text.value(), TRIM_ALL, output);
#endif
}
