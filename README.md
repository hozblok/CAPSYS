# CAPSYS

CAPSYS allows to calculate the characteristics of X-rays as it passes through the capillary structure. The method is based on the geometrical optics approximation. The typical dimensions of capillary structures (tube diameter d) are in the range from 0.1 microns to thousands of microns or more. Tube length is much greater than the diameter (Eg L = 100 mm, d = 0,01 mm).

CAPSYS takes input data and accumulates output data in a SQLite database. There is the development of a graphical user interface to simplify the management of simulation processes.

Additional application CAPSYS_graph created to visualize the data using the MathGL library (http://mathgl.sourceforge.net/).

## Installation
If you do not have information about how to use CAPSYS, please contact the developer first.

Get and build this development tools and libraries:
* [Qt 5.0+ with Qt Creator](https://www.qt.io/download-open-source/) and [MinGW](https://sourceforge.net/projects/mingw/files/)
* [The GNU Multiple Precision Arithmetic Library 6.1.0+](https://gmplib.org/)
* [The GNU MPFR Library 3.1.4+](http://www.mpfr.org/)
* [MathGL 2.3.5+](http://mathgl.sourceforge.net)

Open the [CAPSYS.PRO](/CAPSYS.pro) file (and the [CAPSYS_graph.pro](CAPSYS_graph/CAPSYS_graph.pro)) with Qt Creator. Build and run CAPSYS.

## Examples

## License
CAPSYS is open-sourced software licensed under the [GNU GPL v3](https://opensource.org/licenses/GPL-3.0).
