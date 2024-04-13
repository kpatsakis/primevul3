int chtod(char ch) {
  if (ch >= '0' && ch <= '9')
    return ch - '0';
  else if (ch >= 'a' && ch <= 'z')
    return 10 + ch - 'a';
  else if (ch >= 'A' && ch <= 'Z')
    return 10 + ch - 'A';
  else return -1;
}
