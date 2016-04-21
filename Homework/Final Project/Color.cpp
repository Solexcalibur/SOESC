#include "Color.h"
using namespace std;

Color::Color() {
	r = 0.0;
	g = 0.0;
	b = 0.0;
	a = 1.0;
}

Color::Color(float red, float blue, float green, float alpha)
{
	r = red; b = blue; g = green; a = alpha;
}
