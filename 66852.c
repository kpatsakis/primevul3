static NO_INLINE void jsvFreePtrInternal(JsVar *var) {
  assert(jsvGetLocks(var)==0);
  var->flags = JSV_UNUSED;
  jshInterruptOff(); // to allow this to be used from an IRQ
  jsvSetNextSibling(var, jsVarFirstEmpty);
  jsVarFirstEmpty = jsvGetRef(var);
  touchedFreeList = true;
  jshInterruptOn();
}
