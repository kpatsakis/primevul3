static char** WM_LC_Tokenize_Line(char *line_data) {
    int line_length = (int) strlen(line_data);
    int token_data_length = 0;
    int line_ofs = 0;
    int token_start = 0;
    char **token_data = NULL;
    int token_count = 0;

    if (!line_length) return (NULL);

    do {
        /* ignore everything after #  */
        if (line_data[line_ofs] == '#') {
            break;
        }

        if ((line_data[line_ofs] == ' ') || (line_data[line_ofs] == '\t')) {
            /* whitespace means we aren't in a token */
            if (token_start) {
                token_start = 0;
                line_data[line_ofs] = '\0';
            }
        } else {
            if (!token_start) {
                /* the start of a token in the line */
                token_start = 1;
                if (token_count >= token_data_length) {
                    token_data_length += TOKEN_CNT_INC;
                    token_data = realloc(token_data, token_data_length * sizeof(char *));
                    if (token_data == NULL) {
                        _WM_GLOBAL_ERROR(__FUNCTION__, __LINE__, WM_ERR_MEM,"to parse config", errno);
                        return (NULL);
                    }
                }

                token_data[token_count] = &line_data[line_ofs];
                token_count++;
            }
        }
        line_ofs++;
    } while (line_ofs != line_length);

    /* if we have found some tokens then add a null token to the end */
    if (token_count) {
        if (token_count >= token_data_length) {
            token_data = realloc(token_data,
                ((token_count + 1) * sizeof(char *)));
        }
        token_data[token_count] = NULL;
    }

    return (token_data);
}
