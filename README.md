# tiny calculator

It's a tiny calculator.
You can do your four basic operations with it, and little else.
That's because it's really simple right now.
In the future, I might make it more complex!

# how to use

You get your choice of two usage methods.
The first is the classic "just give it an equation as an argument".
It might resemble this:

```
$ ./calc "(div (add 1 2 3) 3)"
2
```

If a REPL is more your style, you can have that, too!
Just don't pass in any arguments:

```
$ ./calc
> (div (add 1 2 3) 3)
2
```

You write your equations as S-Expressions, like in a LISP.

Try it out!
Here are some basic functions to try:

- `add`, to add
- `sub`, to subtract
- `mul`, to multiply
- `div`, to divide

# how to install

First, clone the repo to a directory on your computer.
Next run `./compile.sh`.
This will produce an executable called `calc`.
You can then move `calc` to your binary folder if you choose.



