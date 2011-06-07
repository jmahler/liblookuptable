
NAME
----

liblookuptable - Look up tables in C++ using an easy to edit text format.

DESCRIPTION
-----------

This library provides operations for loading, editing, and saving
look up tables.
The format of the table in its text form is designed to be easy
to understand and easy to edit.

Refer to the header files themselves for a specific description
of the operations provided by this library.
There is also a Perl version of this library available
([libtext-lookuptabl-perl][tlut]).

The authors inteded use of this library is to allow a user to edit a
text file representation of a look up table which can then be loaded in
to an embedded controller such as [MegaSquirt][msq].
Additional code would be needed to convert this generic structure to
whatever application specific format is required.

What is a *look up table* and how is it different than a *table*? A
*look up table* is commonly used in embedded controllers to avoid the
use of costly floating pointing operations by looking up a value based
on the input coordiantes. A function with two inputs (f(x, y)) which
would use floating point operations can be represented (with some loss
in precsion) as a table.

In contrast a *table* (or spreadsheet) has any number of columns/rows.
The columns can be of different types. And a table does not try to
represent any sort of function, it just stores data.

STRING FORMAT
-------------

The format of the look up table when stored to a string or file should
look like the example below.

                            rpm
 
                  [1000]   [1500]  [2000]  [2500]
           [100]  14.0     15.5    16.4    17.9
      map  [90]   13.0     14.5    15.3    16.8
           [80]   12.0     13.5    14.2    15.7

The x (across top) and y (left column) coordinates have their values
enclosed in square brackets. All values must be present. And the titles
can only span one line. There can be any number of lines and spaces as
long as the values can be discerned. When saving and restoring a table
the original spacing will not be preserved.

The x values start at offset 0 at the left and increase towards the
right. The y values start at offset 0 at the bottom and increase upward.

AUTHOR
------

Jeremiah Mahler <jmmahler@gmail.com><br>
<http://www.google.com/profiles/jmmahler#about>

COPYRIGHT
---------

Copyright &copy; 2011, Jeremiah Mahler.  All Rights Reserved.<br>
This project is free software and released under
the [GNU General Public License][gpl].

 [gpl]: http://www.gnu.org/licenses/gpl.html
 [msq]: http://www.msextra.com
 [tlut]: https://github.com/jmahler/libtext-lookuptable-perl

