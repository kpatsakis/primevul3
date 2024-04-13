void espruino_snprintf_cb(const char *str, void *userdata) {
  espruino_snprintf_data *d = (espruino_snprintf_data*)userdata;

  while (*str) {
    if (d->idx < d->len) d->outPtr[d->idx] = *str;
    d->idx++;
    str++;
  }
}
