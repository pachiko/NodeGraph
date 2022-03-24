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
- In `tasks.json`, use wildcard `*.cpp` to compile all CPP files for the `-g` switch
- In `tasks.json` and `launch.json` specify the direct path of the executable (`-o` for the former and `program` for the latter)
  - so that it will still compile & run when you run from the `.vscode` folder
- Put executables in a `build` folder so that `.gitignore` will ignore with `build/*`. Do not add `.exe` to the executables since it's not Windows but Ubuntu
- When creating header and CPP files, the class definition in the header file should include fields and method prototypes.
  - The actual method implementation should be in the CPP file.
 ```cpp
 // Header:
 class A {
   int a;
   public:
    const int& getA() const;
 }
 // Implementation:
 const int & A::getA() const {
    return a;
 }
 ```
 - Make long names compact with `using`. Eg. 
 ```cpp
 using PortMap = std::unordered_map<std::string_view, std::unique_ptr<Port>>;
 ```
 - Hashing specialisation can be done by defining custom `struct`s that implement the relevant methods and put them in the template of the container
 ```cpp
 using PortConnections = std::unordered_set<std::reference_wrapper<Port>, HashPort, EqualToPort>;
 
 struct HashPort {
    size_t operator()(const Port& p) const
    {
        return h(p.getName()) ^ static_cast<size_t>(p.portType());
    }

    size_t operator()(const std::reference_wrapper<Port>& p) const
    {   
        return this->operator()(p.get());
    }
};

struct EqualToPort {
    bool operator()(const Port &lhs, const Port &rhs) const 
    {
        return lhs.getName() == rhs.getName() && lhs.portType() == rhs.portType();
    }

    bool operator()(const std::reference_wrapper<Port> &lhs, const std::reference_wrapper<Port> &rhs) const 
    {
        return this->operator()(lhs.get(), rhs.get());
    }
};
 ```
 - It can also be done by template specialisation
 ```cpp
template <> struct std::hash<Port> {
   size_t operator()(const Port& key) const {
       static const std::hash<std::string_view> h;
       return h(key.getName()) ^ static_cast<size_t>(key.portType());
   }
};
template <> struct std::equal_to<Port> {
  bool operator()(const Port &lhs, const Port &rhs) const 
  {
      return lhs.getName() == rhs.getName() && lhs.portType() == rhs.portType();
  }
};
template <> struct std::hash<std::reference_wrapper<Port>> {
    size_t operator()(const std::reference_wrapper<Port>& key) const {
        static const std::hash<Port> portH; 
        const Port& k = key.get();
        return portH(k);
    }
};
template <> struct std::equal_to<std::reference_wrapper<const Port>> {
   bool operator()(const std::reference_wrapper<Port> &lhs, const std::reference_wrapper<Port> &rhs) const 
   {
      static const std::equal_to<Port> portEq; 

      const Port& l = lhs.get();
      const Port& r = rhs.get();

      return portEq(l, r);
   }
};
 ```
