 CALLER_ATTACH Font* SubsetterImpl::Subset(const IntegerSet& glyph_ids) {
  GlyphTablePtr glyph_table =
      down_cast<GlyphTable*>(font_->GetTable(Tag::glyf));
  LocaTablePtr loca_table = down_cast<LocaTable*>(font_->GetTable(Tag::loca));

  FontBuilderPtr font_builder;
  font_builder.Attach(factory_->NewFontBuilder());

  GlyphTableBuilderPtr glyph_table_builder;
  glyph_table_builder.Attach(down_cast<GlyphTable::Builder*>(
      font_builder->NewTableBuilder(Tag::glyf)));
  LocaTableBuilderPtr loca_table_builder;
  loca_table_builder.Attach(down_cast<LocaTable::Builder*>(
      font_builder->NewTableBuilder(Tag::loca)));
  if (glyph_table_builder == NULL || loca_table_builder == NULL) {
    return NULL;
  }

  IntegerList loca_list;
  loca_list.resize(loca_table->NumGlyphs());
  loca_list.push_back(0);
  int32_t last_glyph_id = 0;
  int32_t last_offset = 0;
  GlyphTable::GlyphBuilderList* glyph_builders =
      glyph_table_builder->GlyphBuilders();
  for (IntegerSet::const_iterator i = glyph_ids.begin(), e = glyph_ids.end();
                                  i != e; ++i) {
    int32_t length = loca_table->GlyphLength(*i);
    int32_t offset = loca_table->GlyphOffset(*i);

    GlyphPtr glyph;
    glyph.Attach(glyph_table->GetGlyph(offset, length));

    ReadableFontDataPtr data = glyph->ReadFontData();
    WritableFontDataPtr copy_data;
    copy_data.Attach(font_builder->GetNewData(data->Length()));
    data->CopyTo(copy_data);
    GlyphBuilderPtr glyph_builder;
    glyph_builder.Attach(glyph_table_builder->GlyphBuilder(copy_data));
    glyph_builders->push_back(glyph_builder);

    for (int32_t j = last_glyph_id + 1; j <= *i; ++j) {
      loca_list[j] = last_offset;
    }
    last_offset += length;
    loca_list[*i + 1] = last_offset;
    last_glyph_id = *i;
  }
  for (int32_t j = last_glyph_id + 1; j <= loca_table->NumGlyphs(); ++j) {
    loca_list[j] = last_offset;
  }
  loca_table_builder->SetLocaList(&loca_list);

  for (TableMap::iterator i = font_->Tables()->begin(),
                          e = font_->Tables()->end(); i != e; ++i) {
    if (i->first != Tag::glyf && i->first != Tag::loca) {
      TableBuilderPtr dereference;
      dereference.Attach(
          font_builder->NewTableBuilder(i->first, i->second->ReadFontData()));
    }
  }

  return font_builder->Build();
}
