/*
 * Copyright (C) 2011 Jeremiah Mahler <jmmahler@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// {{{ trim()
/*
 * Trim any preceeding spaces.
 */
string trim(const string s, const bool endp=false)
{
	string t(s);
	int i = 0;
	int len;

	len = t.size();

	while (isspace(t[i++])) {};

	t = t.substr((i - 1), len - (i - 1));

	if (endp) {
		len = t.size();
		for (i = len - 1; i >= 0; i--) {
			if (! isspace(t[i]))
				break;
		}

		t = t.substr(0, i + 1);
	}

	return t;
}
// }}}

// {{{ unbracket()
/*
 * Remove any containing brackets.
 */
string unbracket(const string sin)
{
	string s(sin);
	unsigned int p;

	p = s.find('[', 0);
	if (p != string::npos)
		s = s.erase(p, 1);

	p = s.find(']', 0);
	if (p != string::npos)
		s = s.erase(p, 1);

	return s;
}
// }}}

/**
 * Look up table operations.
 *
 * In the template parameters the first argument defines the
 * type of the values in the table and the second defines the
 * values of the coordinates.  The type of both the x and the y
 * coordinates must be the same.
 */
template<class T, class U>
class LookUpTable {
private:
	int x_size;
	int y_size;
	string x_title;
	string y_title;
	string file_name;
	T* vals;
	U* x_coords;
	U* y_coords;

	/*
	 * Get the position in the vals[] from the x (columns) and y (rows) offsets.
	 */
	int _get_pos(const int x, const int y) const
	{
		return ((y * x_size) + x);
	}

public:

	// {{{ LookUpTable(_x_size, ...)
	/**
	 *  Build a new blank LookUpTable.
	 *
	 *  The dimensions of a table can only be defined at construction.
	 *  The values, coordiantes and titles can be changed afterwards.
	 */
	LookUpTable(const unsigned int _x_size, const unsigned int _y_size,
					const string _x_title="", const string _y_title="")
	{
		int i = 0;

		x_size  = _x_size;
		y_size  = _y_size;
		x_title = _x_title;
		y_title = _y_title;

		vals = new T[x_size * y_size];
		x_coords = new U[x_size];
		y_coords = new U[y_size];

		for (i = 0; i < (x_size * y_size); i++) {
			vals[i] = 0;
		}
		for (i = 0; i < x_size; i++) {
			x_coords[i] = 0;
		}
		for (i = 0; i < y_size; i++) {
			y_coords[i] = 0;
		}
	};
	// }}}

	// {{{ LookUpTable (copy constructor)
	/**
	 * Copy constructor.
	 */
	LookUpTable(const LookUpTable& a) {
		x_size = a.x_size;
		y_size = a.y_size;
		x_title = a.x_title;
		y_title = a.y_title;
		file_name = a.file_name;

		vals = new T[x_size * y_size];
		x_coords = new U[x_size];
		y_coords = new U[y_size];

		int i;
		for (i = 0; i < (x_size * y_size); i++) {
			vals[i] = a.vals[i];
		}
		for (i = 0; i < x_size; i++) {
			x_coords[i] = a.x_coords[i];
		}
		for (i = 0; i < y_size; i++) {
			y_coords[i] = a.y_coords[i];
		}
	};
	// }}}

	// {{{ operator=
	/**
	 * Assignment operator.
	 */
	LookUpTable<T, U> operator=(const LookUpTable<T, U>& a) {
		x_size = a.x_size;
		y_size = a.y_size;
		x_title = a.x_title;
		y_title = a.y_title;
		file_name = a.file_name;

		int i;
		for (i = 0; i < (x_size * y_size); i++) {
			vals[i] = a.vals[i];
		}
		for (i = 0; i < x_size; i++) {
			x_coords[i] = a.x_coords[i];
		}
		for (i = 0; i < y_size; i++) {
			y_coords[i] = a.y_coords[i];
		}

		return *this;
	};
	// }}}

	// {{{ ~LookUpTable()
	~LookUpTable()
	{
		//if (NULL != vals) {
			delete[] vals;
			delete[] x_coords;
			delete[] y_coords;

		//	vals = NULL;
		//}
	};
	// }}}

	// {{{ *_title()
	/**
	 * Get the title for the x axis.
	 * 
	 * @returns title
	 */
	string get_x_title()
	{
		return x_title;
	}

	/**
	 * Get the title for the y axis.
	 * 
	 * @returns title
	 */
	string get_y_title()
	{
		return y_title;
	}

	/**
	 * Set the title for the x axis.
	 * 
	 */
	void set_x_title(const string new_title)
	{
		x_title = new_title;
	}

	/**
	 * Set the title for the y axis.
	 * 
	 */
	void set_y_title(const string new_title)
	{
		y_title = new_title;
	}
	// }}}

	// {{{ *_coords()
	/**
	 * Get the coordinate on the x axis.
	 * 
	 * @returns coordiante on success, coordinate at 0 on error
	 */
	U get_x_coord(const int x)
	{
		if (x > x_size)
			return x_coords[0]; // error

		return x_coords[x];
	}

	/**
	 * Get the coordinate on the y axis.
	 * 
	 * @returns coordiante on success, coordinate at 0 on error
	 */
	U get_y_coord(const int y)
	{
		if (y > y_size)
			return y_coords[0]; // error

		return y_coords[y];
	}

	/**
	 * Set the coordinate on the x axis.
	 * 
	 * @returns true on success, false on error (out of bounds)
	 */
	bool set_x_coord(const int x, const U val)
	{
		if (x > x_size)
			return false; // error

		x_coords[x] = val;

		return true;  // OK
	}

	/**
	 * Set the coordinate on the y axis.
	 * 
	 * @returns true on success, false on error (out of bounds)
	 */
	bool set_y_coord(const int y, const U val)
	{
		if (y > y_size)
			return false; // error

		y_coords[y] = val;

		return true;  // OK
	}

	// }}}

	// {{{ get_*_size()
	/**
	 * Get the size of the x axis.
	 * 
	 * @returns size
	 */
	int get_x_size()
	{
		return x_size;
	}

	/**
	 * Get the size of the y axis.
	 * 
	 * @returns size
	 */
	int get_y_size()
	{
		return y_size;
	}

	// }}}

	// {{{ get(x, y)
	/**
	 * get a value in the table by using x and y offsets.
	 *
	 * @returns true on success, value at position 0 on error
	 */
	T get(const int x, const int y) const
	{
		int pos;

		if (x >= x_size || y >= y_size)
			return vals[0];  // error

		pos = _get_pos(x, y);

		return vals[pos];
	};
	// }}}

	// {{{ set(x, y, T)
	/**
	 * set a value in the table by using x and y offsets.
	 *
	 * @returns true on success
	 */
	bool set(const int x, const int y, const T val)
	{
		unsigned int pos;

		if (x < 0 || y < 0 || x >= x_size || y >= y_size)
			return false;

		pos = _get_pos(x, y);

		vals[pos] = val;

		return true;  // OK
	};
	// }}}

	// {{{ lookup(x_val, y_val)
	/**
	 * Look up a value in the table by using x and y values.
	 *
	 * @returns position on success
	 *
	 * When looking up values the position is not absolute unless
	 * the coordinates are exact.
	 * In nearly all situations the position is a fraction between
	 * its neighbor values.
	 * And there are also situations when it is on the edge of the
	 * map these values are undefined.
	 *
	 \verbatim

	    a | b | c
	    --+--+---
	    d | X | f
	    --+--+---
	    g | h | i

	 \endverbatim
	 *
	 * To resolve this situation a set of all values is returned
	 * along with a weight to describe the contribution of each
	 * value to the final value.
	 */
	T lookup(const int x, const int y) const
	{
		int pos;

		if (x >= x_size || y >= y_size)
			return vals[0];  // error

		pos = _get_pos(x, y);

		return vals[pos];
	};
	// }}}

	// {{{ operator<<
	friend ostream& operator<<(ostream& out, const LookUpTable<T, U>& lut)
	{
		/*
		 * All values must be formatted in to evenly spaced columns.
		 * Titles should be centered.
		 */

		/*
		 * The concept is to place everything and then add space as needed
		 * so that everything is even.
		 */

		// all rows except the x title
		int row, col;
		string vals[lut.y_size + 1][lut.x_size + 2];

		// {{{ place all the elements

		// x coordinates
		//    [123] [34] [554]
		vals[0][0] = " ";
		vals[0][1] = " ";
		for (int col = 2; col < lut.x_size + 2; col++) {
			stringstream ss;
			ss << " [" << lut.x_coords[col -2] << "] ";
			vals[0][col] = ss.str();
		}

		for (row = 1; row < lut.y_size + 1; row++) {
			if (row == (lut.y_size / 2)) {
				// y title
				stringstream ss;
				ss << " " << lut.y_title << "  ";

				vals[row][0] = ss.str();
			} else {
				// y title space
				vals[row][0] = " ";
			}

			// y coordiantes
			{
			stringstream ss;
			ss << "[" << lut.y_coords[row - 1] << "] ";
			vals[row][1] = ss.str();
			}

			for (col = 2; col < lut.x_size + 2; col++) {
				stringstream ss;
				ss << " " << lut.get(col - 2, row - 1) << " " ;

				vals[row][col] = ss.str();
			}
		}
		// }}}

		// {{{ space fill columns
		/*
		 * Expand all columns so that they are the same width
		 */

		int width = 0;  // find maximum width

		for (col = 0; col < lut.x_size + 2; col++) {
			// find the longest string, then make all the
			// strings in the column this same length.
			int longest = 0;
			for (row = 0; row < lut.y_size + 1; row++) {
				int len = vals[row][col].size();
				if (len > longest)
					longest = len;
			}
			width += longest;

			for (row = 0; row < lut.y_size + 1; row++) {
				int d = longest - vals[row][col].size();
				stringstream ss;

				//ss << vals[row][col]; // left align, space right

				for (int k = 0; k < d; k++)
					ss << " ";

				ss << vals[row][col]; // right align, space left

				vals[row][col] = ss.str();
			}
		}
		// }}}

		// {{{ output x title

		out << "\n"; // blank line above y title
		{
			int d = width - lut.x_title.length();
			d /= 2;
			d += lut.y_title.length();

			for (int k = 0; k < d; k++)
				out << " ";

			out << lut.x_title << "\n";

		}
		out << "\n"; // blank line below y title
		// }}}

		// {{{ output all the elements
		for (row = 0; row < lut.y_size + 1; row++) {
			for (col = 0; col < lut.x_size + 2; col++) {
				out << vals[row][col];
			}
			out << "\n";
		}
		/// }}}

		return out;
	};
	// }}}

	// {{{ load(string)
	/**
	 * Assign the values of the table by using the definition in the string.
	 *
	 * If the dimensions are different an error will occur.
	 * 
	 * @returns true on success, false on error
	 */
	bool load(const string str_table)
	{
		stringstream ss(str_table);
		string line;
		string _x_title;
		string _y_title;

		getline(ss, line); // blank line above x title
		getline(ss, _x_title); // x title
		_x_title = trim(_x_title);
		x_title = _x_title;
		getline(ss, line); // blank line below x title

		// {{{ x coordinates
		//     [0] [10] ...
		getline(ss, line);
		{
		stringstream ss(line);
		U c;
		string s;

		for (int i = 0; i < x_size; i++) {
			ss >> s;

			s = unbracket(s);

			stringstream sx(s);
			sx >> c;

			set_x_coord(i, c);
		}
		}
		// }}}

		// {{{ y title, y coordinates, values
		string s;
		bool title_found = false;
		for (int i = 0; i < y_size; i++) { // for every row
			getline(ss, line);

			if (! title_found) {
				unsigned int p = line.find('[', 0);
				if (p == string::npos) {
					cerr << "missing '[' bracket to denote y coordinates\n";
					return false;
				}

				for (unsigned int k = 0; k < p; k++) {
					if (isalpha(line[k])) {
						title_found = true;
						break;
					}
				}

				if (title_found) {
					_y_title = line.substr(0, p);

					_y_title = trim(_y_title, true);

					y_title = _y_title;

					line = line.erase(0, (p - 1));
				}
			}

			stringstream sx(line);

			// y coordiate
			{
			U yc;
			sx >> s;
			s = unbracket(s);
			stringstream sz(s);
			sz >> yc;

			set_y_coord(i, yc);
			}

			// values
			T val;
			for (int x = 0; x < x_size; x++) {
				sx >> val;
				
				set(x, i, val);
			}


		}
		// }}}

		return true;  // OK
	}
	// }}}

	// {{{ set_file(file)
	/**
	 * Set the current file name.
	 *
	 * @arg file name
	 */
	void set_file(const string new_file_name)
	{
		file_name = new_file_name;
	}
	// }}}

	// {{{ get_file()
	/**
	 * Get the current file name.
	 *
	 * @returns file name
	 */
	string get_file()
	{
		return file_name;
	}
	// }}}

	// {{{ load_file(file)
	/**
	 * Assign the values of the table by using the definition from a file.
	 * Works just like load() except the string is loaded from the file.
	 * If the file is not specified it uses the last known file.
	 * 
	 * @returns true on success, false on error
	 */
	bool load_file(const string file="")
	{
		string content;
		string line;
		bool res;

		if (! file.empty()) {
			file_name = file;
		}
		if (file_name.empty())
			return false;  // error

		ifstream from(file_name.c_str());
		if (!from)
			return false;  // error

		while (getline(from, line)) {
			content += line + "\n";
		}

		res = load(content);

		return res;
	}
	// }}}

	// {{{ save(file_name)
	/**
	 * Save the table to the given file name.
	 * If no file name is given it uses the last known file name.
	 * 
	 * @returns true on success, false on error
	 */
	bool save(const string new_file_name="") {

		if (! new_file_name.empty()) {
			file_name = new_file_name;
		}
		if (file_name.empty()) {
			cerr << "LookUpTable::save(), error, no file name\n";
			return false;  // error
		}

		ofstream to(file_name.c_str());
		if (!to) {
			cerr << "LookUpTable::save(), error trying to open file\n";
			return false;  // error
		}

		to << (*this) << endl;

		return true; // OK
	}
	// }}}

	// {{{ operator==
	/**
	 * Test whether one object is equal to the other.
	 *
	 * @returns true if same, false otherwise
	 *
	 * Currently only tests the values and does not test
	 * the coordinates.
	 */
	friend bool operator==(const LookUpTable<T, U>& a, const LookUpTable<T, U>& b)
	{
		if (a.x_size != b.x_size || a.y_size != b.y_size)
			return false;

		int size = a.x_size * a.y_size;
		for (int i = 0; i < size; i++) {
			if (a.vals[i] != b.vals[i])
				return false;
		}

		return true;
	};
	// }}}

	// {{{ operator!=
	/**
	 * Test whether one object is NOT equal to the other.
	 *
	 * @returns true if not equal, false otherwise
	 */
	friend bool operator!=(const LookUpTable<T, U>& a, const LookUpTable<T, U>& b)
	{
		return (! (a == b));
	};
	// }}}


};
