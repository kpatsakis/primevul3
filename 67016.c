void itostr_extra(JsVarInt vals,char *str,bool signedVal, unsigned int base) {
  JsVarIntUnsigned val;
  if (signedVal && vals<0) {
    *(str++)='-';
    val = (JsVarIntUnsigned)(-vals);
  } else {
    val = (JsVarIntUnsigned)vals;
  }
  JsVarIntUnsigned tmp = val;
  int digits = 1;
  while (tmp>=base) {
    digits++;
    tmp /= base;
  }
  int i;
  for (i=digits-1;i>=0;i--) {
    str[i] = itoch((int)(val % base));
    val /= base;
  }
  str[digits] = 0;
}
