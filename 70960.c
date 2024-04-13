static int get_second_size(char *codec_name)
{
    int result = -1;

    if (!strcmp(codec_name, "mp332")) {
        result = 3982;
    } else if (!strcmp(codec_name, "acelp16")) {
        result = 2000;
    } else if (!strcmp(codec_name, "acelp85")) {
        result = 1045;
    }

    return result;
}
