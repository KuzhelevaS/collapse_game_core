#ifndef CUBES_H
#define CUBES_H

#include <vector>

namespace cubes {
	enum class Status {
		NORMAL,
		ACTIVE,
		DISABLE
	};
	enum class Color {
		WHITE,
		BLACK,
		RED,
		BLUE,
		YELLOW,
		ORANGE,
		GREEN,
		VIOLET,
		DARK_GREEN,
		LIGHT_GREEN,
		DARK_VIOLET,
		LIGHT_VIOLET,
		DARK_ORANGE,
		LIGHT_ORANGE,
		PINK,
		SKY,
		LIME,
		BROWN
	};

	using ColorSet = std::vector<Color>;

	enum class Bonuses {
		REMOVE_CELL, // за схлопывание n ячеек
		CHANGE_CELL_COLOR, // x5 за очистку всего поля
		CLEAR_SAME_COLOR // за сбор картинки
	};
}


#endif // CUBES_H
