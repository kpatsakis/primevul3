void re_yyset_lineno (int  _line_number , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

        /* lineno is only valid if an input buffer exists. */
        if (! YY_CURRENT_BUFFER )
           YY_FATAL_ERROR( "re_yyset_lineno called with no buffer" );
    
    yylineno = _line_number;
}
