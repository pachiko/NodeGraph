# NodeGraph
AutoDesk

# Lessons learnt:
- String literals have static duration (lasts the program lifetime)
- Try not to pass `string` as input to functions, since many copies will be made
- `string_view` is a reference to a `string`. So it can be dangling (rubbish)
  - Eg when the `name` of a `Node` is a `string`, and the `hash_map`'s key uses that `Node`'s `name`. If the `Node` goes out of scope, GG
- The `[]` operator of a `hash_map` will create an element by `default` CTOR if the unique key is not present
- The elements in a `set` are always `const &`. You can't modify the elements, to prevent hashing and equality invalidness. Use a `hash_map`
- `make_shared` & `make_unique` does not need to receive the output of `new`, can just specify the args and it will call the CTOR
  - If the former is done, compiler expects a CTOR that receives a `ptr` as input eg. `Port(Port* p)`
