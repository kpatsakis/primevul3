static void end_hexline(void)
{
    if (hexline_length == HEXLINE_WIDTH) {
        fputs("\n", bitfile);
        hexline_length = 0;
    }
}
