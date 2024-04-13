void jswrap_graphics_fillPoly(JsVar *parent, JsVar *poly) {
  JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
  if (!jsvIsIterable(poly)) return;
  const int maxVerts = 128;
  short verts[maxVerts];
  int idx = 0;
  JsvIterator it;
  jsvIteratorNew(&it, poly, JSIF_EVERY_ARRAY_ELEMENT);
  while (jsvIteratorHasElement(&it) && idx<maxVerts) {
    verts[idx++] = (short)jsvIteratorGetIntegerValue(&it);
    jsvIteratorNext(&it);
  }
  jsvIteratorFree(&it);
  if (idx==maxVerts) {
    jsWarn("Maximum number of points (%d) exceeded for fillPoly", maxVerts/2);
  }
  graphicsFillPoly(&gfx, idx/2, verts);
  graphicsSetVar(&gfx); // gfx data changed because modified area
}
