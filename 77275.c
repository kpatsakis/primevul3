base::PropertyAccessor<AutocompleteEditState>* GetStateAccessor() {
  static base::PropertyAccessor<AutocompleteEditState> state;
  return &state;
}
