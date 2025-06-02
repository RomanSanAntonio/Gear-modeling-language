# GML
This is a simple easy to read gear system modeling language with features including:
- simple gear ratio sim with NM and RPM
- easy to learn and to understand syntax
- friction and heat through customizable friction functions
- C++ transpiler to create custom features unique to your needs

## Syntax
To create a gear, just initialize it with ```name = amount-of-teeth```. 
example: 
``` GML
gear1 = 14
gear2 = 28
```
To connect them together with their teeth, use an underscore method like this: ```gear1_c_gear2```
example:
``` GML
g1 = 8
g2 = 64
g3 = 32
g1_c_g2_c_g3
```
All of the underscore methods are listed here:
| Operation | Function                             |
|-----------|--------------------------------------|
| `_s_`     | where the power is inputted          |
| `_c_`     | connects the gears together          |
| `_i_`     | makes the gears be on the same axle  |
| `_o_`     | displays RPM and NM at end of sim    |
| `_b_`     | moves back through the line          |
| `_f_`     | moves forward through the line       |
| `_p_`     | makes it spin clockwise              |
| `_q_`     | makes it spin counter-clockwise      |

Example:
``` GML
g1 = 8
g2 = 128
g3 = 8
g4 = 128
g5 = 8
_p_g1_i_g2_c_g3_i_g4_c_o_g5
RPM:2
NM:100
```
Output:
``` GML
g5:
  Spin: Clockwise
  RPM:512
  NM:0.39
```
## Friction
These atributes are added to the end of an initialiser in square brakets ```[]``` and have these properties

- tempurature
- friction function
- resistance
- special functionality

**ORDER MATTERS!** Anything that is out of order will be trated as a syntax error.

The atributes go in this order, ```[h = degreeC, f = frictionfunc, r = resistance, id = "text", spcl={C++ code}]```

*note that you don't have to list all of the keywords*

Friction is calculated like this:

```if f(RPM) >= NM: {RPM = 0;} elif f(RPM) < NM: {NM - f(RPM);}```

When f(x) is the friction equation.

The heat is calculated like so, 

$$h_o = \frac{RPM}{2} + \frac{NM_L}{10} + h_s$$

Where $$h_o$$ is the result, $$NM_L$$ is the loss of NM from friction, and $$h_s$$ is the heat at the start.

Also every tick the game spreads the heat out like heat propogation. calculated like this, avg(heatpre, heat, heatnext) it just averages the heat together.
If a peice is on the edge it will have only 2 elements to average toether.

## Code Blocks

**DISCLAIMER!** These code blocks are written in C++. If you do not know C++, I highly suggest learning that before learning the rest of GML. 
You are still enabled to use GML without C++, but to get the full functionallity of GML, please learn C++.

These code blocks can be placed anywhere inside of curly braces `{}`

Example:

``` GML
{
  void Fire(str item) {
    if tick % 4 == 0: {
      item.RPM += 20;
    }
  }
  void start(str item) {
    if key_press.up:{
      item.RPM += 10;
    }
  }
}
s1 = 8[h = 10 id = "starter" spcl = {start("starter")}]
s2 = 128[h = 10]
p = 1[h = 10, id = "piston", spcl = {Fire("crankshaft");}]
cs = 0[h = 10, id = "crankshaft"]
_s_s1_c_s2_i_cs_i_o_p
```

This is a small 1 piston engine. If you press the up arrow key will start and rev forever. Not too interesting, but you get the idea.
These code blocks can let you add anything to GML, even extend it within itself, but I will leave that up for you to do.



A GML interpreter is on the way!!!
