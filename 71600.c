static void Np_toExponential(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	int width = js_tointeger(J, 1);
	char buf[32];
	double x;
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	if (width < 0) js_rangeerror(J, "precision %d out of range", width);
	if (width > 20) js_rangeerror(J, "precision %d out of range", width);
	x = self->u.number;
	if (isnan(x) || isinf(x))
		js_pushstring(J, jsV_numbertostring(J, buf, x));
	else
		numtostr(J, "%.*e", width, self->u.number);
}
