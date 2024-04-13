const char *escapeCharacter(char ch) {
  if (ch=='\b') return "\\b";
  if (ch=='\f') return "\\f";
  if (ch=='\n') return "\\n";
  if (ch=='\a') return "\\a";
  if (ch=='\r') return "\\r";
  if (ch=='\t') return "\\t";
  if (ch=='\\') return "\\\\";
  if (ch=='"') return "\\\"";
  static char buf[5];
  if (ch<32 || ch>=127) {
    /** just encode as hex - it's more understandable
     * and doesn't have the issue of "\16"+"1" != "\161" */
    buf[0]='\\';
    buf[1]='x';
    int n = (ch>>4)&15;
    buf[2] = (char)((n<10)?('0'+n):('A'+n-10));
    n=ch&15;
    buf[3] = (char)((n<10)?('0'+n):('A'+n-10));
    buf[4] = 0;
    return buf;
  }
  buf[1] = 0;
  buf[0] = ch;
  return buf;
}
