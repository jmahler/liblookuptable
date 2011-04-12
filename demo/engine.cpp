
#include <iostream>
#include <string>
#include <cmath>  // abs()

#include <unistd.h>

#include "lookuptable.h"

using namespace std;

int main()
{
	string tbl_str("timing.tbl");
	bool ok;

	// {{{ load previous table OR build new from default
	LookUpTable<float, int> lut(12, 12);

	ok = lut.load_file(tbl_str);
	if (! ok) {
		// build and save the default table
		// These are just rudimentary starting values.

		lut.set_x_title("rpm");
		lut.set_y_title("map (%)");

		for (int i = 0; i < 12; i++) {
			lut.set_x_coord(i, i*1000 + 1000);
		}

		for (int i = 0; i < 12; i++) {
			lut.set_y_coord(i, (i*(-100/11) + 100));
		}

		for (int x = 0; x < 12; x++) {
			for (int y = 0; y < 12; y++) {
				int xc = lut.get_x_coord(x);
				int yc = lut.get_y_coord(y);

				// a linear map
				// c_1 * x + c_2 * y = z
				float val = xc*0.0017474332648871 + yc*0.0030800821355236 + 5;

				lut.set(x, y, val);
				//lut.set(x, y, x+y);
			}
		}

		lut.save(tbl_str);
	}
	// }}}

	// {{{ running engine
	/*
	 * This is a very crude attempt at simulating
	 *  the behaviour of an engine.
	 *
	 * The main requirement is that the rpm stabilizes
	 *  at some point depending on the throttle (map) position.
	 * And that suboptimal ignition timing will result in reduced power.
	 *
	 * It should be useful enough to test optimization programs.
	 */

	float rpm0 = 1000;  // t0
	float rpm1; // t1
	float map = 100;
	float m = -109.0/110.0;
	float b = -100.0/11.0;
	float ign; // ideal ignition timing
	float igna; // actual ignition timing

	while (1) {
		cout << "rpm, map: " << rpm0 << ", " << map << endl;

		// calculate ideal ignition timing (max power)
		ign = (1.0/220.0)*rpm0 + (-1.0/10.0)*map + (60.0/11.0);

		igna = ign;  // TODO look up value

		// reduce power if timing is suboptimal
		map -= abs(ign - igna);

		rpm1 = map - (m*rpm0 + b);

		rpm0 = rpm1;

		//usleep(1000000);
		//usleep(50000);
		usleep(500);
	}
	// }}}

	return 0;  // ok
}
