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

# advanced functions

There are a handful of more advanced functions you can use as well.

<!--
## descriptive statistics
- `mean`
- `sd`, to get the standard deviation of some data
- `var`, to get the variance

## probability
- `norm-pdf`, which is the normal distribution's pdf given `x` and `mu` and `sigma`
- `norm-cdf`, to calculate probability of a value less than or equal to some `x` with the respective `mu` and `sigma`
- `bin-pmf`, to calculate probability of a certain binomial result given `n` and `k`
- `pos-pmf`, to calculate probability of a certain Poisson result given `k` and `lambda`
-->

## actuarial functions
- `fv`, to calculate the future value of one dollar at rate `i` after `n` periods
- `pv`, to calculate the present value of what will be worth one dollar at a rate `i` after `n` periods
- `ai`, to calculate the present value of an annuity immediate that pays one dollar at the end of each period, at rate `i` over `n` periods
- `ad`, to do the same as `ai` but for an annuity due
- `si`, to calculate the future value of an annuity immediate that pays one dollar at the end of each period, at rate `i` over `n` periods
- `sd`, to do the same as `si` but for annuity due
- `perp`, to calculate the present value of a perpetuity that pays 1 every period, at rate `i`
- `itodelta`, to calculate the force of interest given rate `i`
- `deltatoi`, to calculate effective rate given force of interest `delta`
- `itod`, to calculate discount rate given interest rate `i`
- `dtoi`, to calculate interest rate given discount rate `i`
- `bond`, to calculate the price of a bond given its face value `fv`, coupon rate `r`, and term `n`



