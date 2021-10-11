# clox

clox is a programming language which is being built while reading the book [Crafting Interpreters](https://craftinginterpreters.com).

## Building

Two things are necessary to build clox:

- cmake
- gcc

Run the following command to generate all necessary files for cmake to build the project:

```
cmake -H. -Bbuild
```

Afterwards you can run the following command to build the clox executable:

```
cmake --build build
```
