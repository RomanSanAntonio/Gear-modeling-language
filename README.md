# GML
This is a simple easy to read gear system modeling language with features including:
- simple gear ratio sim with NM and RPM
- easy to learn and to understand syntax
- friction and heat through customizable friction functions

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
RPM:g1=2
NM:g1=100
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

Also every tick the game spreads the heat out like heat propogation. calculated like this, avg(heatpre, heat, heatnext) it just 
averages the heat together.
If a peice is on the edge it will have only 2 elements to average toether.

A GML interpreter is on the way!!!
