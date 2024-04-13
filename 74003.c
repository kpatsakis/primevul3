void cmd_starttls(char *tag __attribute__((unused)),
                  int imaps __attribute__((unused)))
{
    fatal("cmd_starttls() executed, but starttls isn't implemented!",
          EC_SOFTWARE);
}
