void re_yyset_column (int  _column_no , yyscan_t yyscanner)
{
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner;

        /* column is only valid if an input buffer exists. */
        if (! YY_CURRENT_BUFFER )
           YY_FATAL_ERROR( "re_yyset_column called with no buffer" );
    
    yycolumn = _column_no;
}
