
#include "LookUpTable.h"
#include "LUTPoint.h"

#include <assert.h>
#include <iostream>

using namespace std;

int main()
{

	// {{{ construct some simple tables
	{
	LookUpTable<int, int> lut1(11, 10);
	assert(11 == lut1.get_x_size());
	assert(10 == lut1.get_y_size());

	LookUpTable<float, int> lut2(11, 10);
	assert(11 == lut2.get_x_size());
	assert(10 == lut2.get_y_size());

	LookUpTable<float, float> lut3(11, 10);
	assert(11 == lut3.get_x_size());
	assert(10 == lut3.get_y_size());
	}
	// }}}

	// {{{ set/get values
	{
	LookUpTable<int, int> lut(11, 10);
	assert(lut.set(3, 2, -10));
	assert(lut.set(2, 3, 10));
	assert(lut.set(2, 2, 4));

	assert(-10 == lut.get(3, 2));
	assert(10 == lut.get(2, 3));
	assert(4 == lut.get(2, 2));
	}
	// }}}

	// {{{ table out/in
	{
	LookUpTable<int, int> lut(10, 10, "really long x title", "y (mm) ojojwef");
	assert(lut.set(0, 3, 03));
	assert(lut.set(3, 3, 33));
	assert(lut.set(2, 3, 23));
	assert(lut.set(1, 3, 13));
	assert(lut.set(5, 5, 55));
	assert(lut.set(6, 6, 66));
	assert(lut.set(5, 9, 59));

	int sz = lut.get_x_size();
	for (int i = 0; i < sz; i++) {
		assert(lut.set_x_coord(i, i));
	}

	sz = lut.get_y_size();
	for (int i = 0; i < sz; i++) {
		assert(lut.set_y_coord(i, i + 95));
	}

	stringstream ss;
	ss << lut;

	//cout << lut ;

	LookUpTable<int, int> lut_in(10, 10);

	assert(lut_in.load(ss.str()));

	//cout << lut_in ;

	assert(3 == lut_in.get(0, 3));
	assert(33 == lut_in.get(3, 3));
	assert(23 == lut_in.get(2, 3));
	assert(13 == lut_in.get(1, 3));
	assert(55 == lut_in.get(5, 5));
	assert(66 == lut_in.get(6, 6));
	assert(59 == lut_in.get(5, 9));
	}
	// }}}

	cout << "All tests passed.\n";

	return 0;
}
