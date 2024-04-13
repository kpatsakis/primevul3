GURL FilePathToFileURL(const FilePath& path) {
  FilePath::StringType url_string(kFileURLPrefix);
  url_string.append(path.value());


  ReplaceSubstringsAfterOffset(&url_string, 0,
      FILE_PATH_LITERAL("%"), FILE_PATH_LITERAL("%25"));

  ReplaceSubstringsAfterOffset(&url_string, 0,
      FILE_PATH_LITERAL(";"), FILE_PATH_LITERAL("%3B"));

  ReplaceSubstringsAfterOffset(&url_string, 0,
      FILE_PATH_LITERAL("#"), FILE_PATH_LITERAL("%23"));

#if defined(OS_POSIX)
  ReplaceSubstringsAfterOffset(&url_string, 0,
      FILE_PATH_LITERAL("\\"), FILE_PATH_LITERAL("%5C"));
#endif

  return GURL(url_string);
}
