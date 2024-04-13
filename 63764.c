static void enc_getline(void)
{
    char *p;
    int c;
  restart:
    if (enc_eof())
        pdftex_fail("unexpected end of file");
    p = enc_line;
    do {
        c = enc_getchar();
        append_char_to_buf(c, p, enc_line, ENC_BUF_SIZE);
    } while (c != 10);
    append_eol(p, enc_line, ENC_BUF_SIZE);
    if (p - enc_line < 2 || *enc_line == '%')
        goto restart;
}
