std::string SkColorToRGBAString(SkColor color) {
  return "rgba(" + base::IntToString(SkColorGetR(color)) + "," +
      base::IntToString(SkColorGetG(color)) + "," +
      base::IntToString(SkColorGetB(color)) + "," +
      base::DoubleToString(SkColorGetA(color) / 255.0) + ")";
}
