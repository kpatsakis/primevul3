static void Np_valueOf(js_State *J)
{
	js_Object *self = js_toobject(J, 0);
	if (self->type != JS_CNUMBER) js_typeerror(J, "not a number");
	js_pushnumber(J, self->u.number);
}
