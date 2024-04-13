static void jsB_new_Number(js_State *J)
{
	js_newnumber(J, js_gettop(J) > 1 ? js_tonumber(J, 1) : 0);
}
