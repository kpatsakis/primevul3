 void jswrap_graphics_fillCircle(JsVar *parent, int x, int y, int rad) {
   JsGraphics gfx; if (!graphicsGetFromVar(&gfx, parent)) return;
   graphicsFillCircle(&gfx, (short)x,(short)y,(short)rad);
   graphicsSetVar(&gfx); // gfx data changed because modified area
 }
