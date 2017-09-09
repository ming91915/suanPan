Coding Guideline
================

Naming Convention
-----------------

`suanPan` basicly follows [Bjarne Stroustrup's C++ Style](http://www.stroustrup.com/bs_faq2.html), in which good reasons of using the sugguested style have been well demostrated. Further considerations are adopted.

### Macro

`ALL_CAPS` is reserved for macros, if necessary, it is also okay to add leading or trailing underscore(s), such as `_MY_NAME_`.

### Class, Structure and Enumerate Class

`UpperCamelCase` (a.k.a. `PascalCase`, see the [link](https://en.wikipedia.org/wiki/PascalCase)) is used for class and structure names. If abbreviation is present in the class name, then the underscore is used as the additional seperator. For example, `ODE_Solver`.

### Variable

In general, all variables should be denoted in `snake_case` (see the [link](https://en.wikipedia.org/wiki/Snake_case)). Abbreviation should be avoided as the modern IDEs have sophiscated auto-complete functionality. Abbreviations can barely improve the coding efficicency. Symbols should be used with caution. For others who are not familar with the backgroud theory, symbols reduce the readability. Following rules can be used as a guideline.

-   `snake_case` is used for (member) (static) (const) variables. All characters should be lower case. The name should have explicit meaning and abbreviation should be avoided. Names like `Cstress` should not be used. However, some common abbreviations are still allowed, such as `tmp_`, `t_` for temporary, `pre_` for previous, `num_` for number, `_ptr` for pointer, `_obj` for object and `idx` for index.
-   Temporary variables (for the purpose of saving unnecessary repeated computation) should be labeled with prefix `tmp_` or `t_`, most of them do not possess physical meanings so it is okay to use `tmp_factor`, `tmp_a`, `tmp_b`, etc. to name those variables.
-   Intermediate variables used during the computation that also exist in the corresponding theory can be labeled with a single character in upper case plus a number, such as `S1`, `S1`, `S2` and `S3`.
-   Physical quantities can be either labeled with its full name or conventional character notation (symbols). For example, it is okay to use `displacement` and `U` to denote displacement. But you are encouraged to use full names as possible as you can. So use `incre_disp` over `dU`, use `current_stress` over `Csigma`.

### Method

Member methods should be denoted in `snake_case` as well. Although `CamelCase` is another very common in some tools, to be consistent with STL, `suanPan` uses `snake_case`. Compare the following two different styles: `tmp_obj.zeros()` and `obj_ptr->get_another()` vs. `tmp_obj.Zeros()` and `obj_ptr->getAnother()`, personally I think the latter is ugly.

### Independent Function

To distinguish functions from methods, it is recommended to use `lowerCamelCase` for independent funtions.

### Formatting

I recommend you to use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) and [clang-tidy](http://clang.llvm.org/extra/clang-tidy/) to check and format your code. They are super elegantly designed tools.

Programming Tips
----------------

### Do not use `new`, `new[]`, `delete` and `delete[]` to manage dynamic variables.

For plain data type, use `std::vector` instead. For objects, use smart pointers instead. By such, you will be free from potential memory leak and dangling pointers. Furthermore, you do not need to provide customized copy/move (assign) constructors according to [rule of five](http://en.cppreference.com/w/cpp/language/rule_of_three) as often you have to release the memory in a customized destructor when using `new`.

### Use `auto` as possible as you can.

Use `auto` for automatic type deduction.

### Use range-based `for` loop.

Use range-based `for` loops when appropriate to access the content directly and avoid bound overflow.

### Use `const` and `&`.

Most methods return a constant reference, some of them also provide a non-const version. Some may even provide a companion friend function to return a modifiable reference. In most cases, the returned variables only need to be read. So it is suggested to use `&` to avoid copy and use `const` as a guard.

``` cpp
const auto& returned_value=object_ptr->get_data_from_method();
```

is more efficient than

``` cpp
auto returned_value=object_ptr->get_data_from_method();
```

if the returned value is expensive to copy.
