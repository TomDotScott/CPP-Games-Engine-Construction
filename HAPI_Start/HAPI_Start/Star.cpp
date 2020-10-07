#include "Star.h"

void Star::Render(HAPISPACE::BYTE* _screen) {
	// Hard coding for now, will fix later...
	for (int x = 0; x < 1000; x++) {
		for (int y = 0; y < 1000; y++) {
			if (x <= m_size + m_position.x && x >= m_position.x && y <= m_size + m_position.y && y >= m_position.y) {
				_screen[(1000 * x + y) * 4] = 255;
				_screen[(1000 * x + y) * 4 + 1] = 219;
				_screen[(1000 * x + y) * 4 + 2] = 38;
				_screen[(1000 * x + y) * 4 + 3] = 255;
			}
		}
	}
}
