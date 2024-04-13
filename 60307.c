void re_yyset_out (FILE *  _out_str , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;
    yyout = _out_str ;
}
