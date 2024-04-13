bool isWhitespace(char ch) {
    return (ch==0x09) || // \t - tab
           (ch==0x0B) || // vertical tab
           (ch==0x0C) || // form feed
           (ch==0x20) || // space
           (ch=='\n') ||
           (ch=='\r');
}
