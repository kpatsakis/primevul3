bool jsvIsRefUsedForData(const JsVar *v) { return jsvIsStringExt(v) || (jsvIsString(v)&&!jsvIsName(v)) ||  jsvIsFloat(v) || jsvIsNativeFunction(v) || jsvIsArrayBuffer(v) || jsvIsArrayBufferName(v); }
