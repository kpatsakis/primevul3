FilePath::StringType StripOrdinalNumber(
    const FilePath::StringType& pure_file_name) {
  FilePath::StringType::size_type r_paren_index =
      pure_file_name.rfind(FILE_PATH_LITERAL(')'));
  FilePath::StringType::size_type l_paren_index =
      pure_file_name.rfind(FILE_PATH_LITERAL('('));
  if (l_paren_index >= r_paren_index)
    return pure_file_name;

  for (FilePath::StringType::size_type i = l_paren_index + 1;
       i != r_paren_index; ++i) {
    if (!IsAsciiDigit(pure_file_name[i]))
      return pure_file_name;
  }

  return pure_file_name.substr(0, l_paren_index);
}
