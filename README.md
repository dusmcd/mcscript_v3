# McScript Programming Language v. 3.0

**How to use**
- In the home directory, run `make main`
- Run the executable at `bin/main`
- This will open a REPL where you can start typing commands (see below for syntax)

**Testing**
- In the home directory, run `make test`
- Test results will be printed to standard output => failures will be printed to standard error

## Syntax and Basic Usage

**Variables**
- Declaring variables:
  ```
  var x = 10;
  var y = true;
  ```
- Variables are dynamically typed

**Loops**
- For loops function the same way as all C-style languages
- Locally scoped
- They are not expressions, i.e., they do not return a value and cannot be passed around
    like functions
```
for (var i = 0; i < 3; i = i + 1) {
    print(i);
}
```

**Types**
- Integers
- Boolean (true and false)
- Strings (e.g., "Hello", "Foo bar")
- Arrays (e.g., `[1, 2, 3], [1, "Hello", true, function(a, b) { a + b }]`)
    - Arrays can be bound to variables

**Control Flow**
```
if (true) {
  return 10;
} else {
  return 11;
}
```
- If expressions will evaluate any expression as truthy or falsey
  - NULL and (boolean) false values are falsey
  - Everything else is truthy

**Functions**
- Function literals can be bound to identifiers (i.e., variable names) or they can be called directly
```
  var add = function(a, b) { return a + b; };
  function(a, b) { a + b; }(1, 2);
```
- Functions can be passed around as arguments and they can be returned from other functions
```
  var newAdder = function(x) {
    return function(a) {
      return a + x;
    }
  };
  var addTwo = newAdder(2);
  addTwo(5);
```

**Built-in Functions**
- print: can print any expression
- len: returns the length of a string or an array
- push: accepts an array and any expression as arguments;
    it will add the second argument to the end of the array

**Currently Supported Operators**
- Infix operators:
  - "+" (addition)
  - "-" (subtraction)
  - "*" (multiplication)
  - "/" (division)
  - "<" (greater than)
  - ">" (less than)
  - "==" (equals)
  - "!=" (not equal)
- Prefix operators:
  - "!" will make truthy expressions falsey and vice versa
  - "-" will make positive numbers negative and vice versa

## Memory Management
- Memory is automatically managed by native garbage collector
