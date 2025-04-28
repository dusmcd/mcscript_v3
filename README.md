# McScript Programming language v. 3.0

**How to use**
- In the home directory, run `make main`
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

**Types**
- Integers
- Boolean (true and false)

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

**Currently Supported Operators**
- Infix operators:
  - "+" (addition)
  - "-" (subtraction)
  - "*" (multiplication)
  - "/" (division)
- Prefix operators:
  - "!" will make truthy expressions falsey and vice versa
  - "-" will make positive numbers negative and vice versa

## Memory Management
- Memory is automatically managed by native garbage collector