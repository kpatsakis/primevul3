void jsB_initnumber(js_State *J)
{
	J->Number_prototype->u.number = 0;

	js_pushobject(J, J->Number_prototype);
	{
		jsB_propf(J, "Number.prototype.valueOf", Np_valueOf, 0);
		jsB_propf(J, "Number.prototype.toString", Np_toString, 1);
		jsB_propf(J, "Number.prototype.toLocaleString", Np_toString, 0);
		jsB_propf(J, "Number.prototype.toFixed", Np_toFixed, 1);
		jsB_propf(J, "Number.prototype.toExponential", Np_toExponential, 1);
		jsB_propf(J, "Number.prototype.toPrecision", Np_toPrecision, 1);
	}
	js_newcconstructor(J, jsB_Number, jsB_new_Number, "Number", 0); /* 1 */
	{
		jsB_propn(J, "MAX_VALUE", 1.7976931348623157e+308);
		jsB_propn(J, "MIN_VALUE", 5e-324);
		jsB_propn(J, "NaN", NAN);
		jsB_propn(J, "NEGATIVE_INFINITY", -INFINITY);
		jsB_propn(J, "POSITIVE_INFINITY", INFINITY);
	}
	js_defglobal(J, "Number", JS_DONTENUM);
}
