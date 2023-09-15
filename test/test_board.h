#include "collapse_game.h"

TEST_CASE("Collapse board") {
	using namespace collapse;
	using detail::GameBoard;
	using cubes::Status;
	SECTION("Constructor, getField, get width") {
		int w = 2, h = 3;
		GameBoard board (w, h, {cubes::Color::YELLOW});
		auto field = board.GetField();
		REQUIRE(field.size() == static_cast<size_t>(w * h));
		CHECK(field.at(0).status == Status::NORMAL);
		CHECK_FALSE(field.at(0).isEmpty);
		CHECK(field.at(0).color == cubes::Color::YELLOW);
		CHECK(field.at(1).status == Status::NORMAL);
		CHECK_FALSE(field.at(1).isEmpty);
		CHECK(field.at(1).color == cubes::Color::YELLOW);
		CHECK(field.at(2).status == Status::NORMAL);
		CHECK_FALSE(field.at(2).isEmpty);
		CHECK(field.at(2).color == cubes::Color::YELLOW);
		CHECK(field.at(3).status == Status::NORMAL);
		CHECK_FALSE(field.at(3).isEmpty);
		CHECK(field.at(3).color == cubes::Color::YELLOW);
		CHECK(field.at(4).status == Status::NORMAL);
		CHECK_FALSE(field.at(4).isEmpty);
		CHECK(field.at(4).color == cubes::Color::YELLOW);
		CHECK(field.at(5).status == Status::NORMAL);
		CHECK_FALSE(field.at(5).isEmpty);
		CHECK(field.at(5).color == cubes::Color::YELLOW);
		CHECK(board.GetWidth() == w);
		CHECK(board.GetHeight() == h);
	}

	SECTION("Random genarator") {
		size_t w = 100, h = 100;
		GameBoard board (w, h, {cubes::Color::YELLOW, cubes::Color::RED, cubes::Color::GREEN, cubes::Color::BLUE});
		auto field = board.GetField();
		unsigned yellow = 0;
		unsigned red = 0;
		unsigned green = 0;
		unsigned blue = 0;
		for (auto cell : field) {
			switch (cell.color) {
				case cubes::Color::YELLOW :
					++yellow;
					break;
				case cubes::Color::RED :
					++red;
					break;
				case cubes::Color::GREEN :
					++green;
					break;
				case cubes::Color::BLUE :
					++blue;
					break;
				default:
					break;
			}
		}
		REQUIRE(red + green + yellow + blue == w * h);
		// ~1/3 для 4х цветов
		CHECK(red < 3000);
		CHECK(red > 0);
		CHECK(green < 3000);
		CHECK(green > 0);
		CHECK(yellow < 3000);
		CHECK(yellow > 0);
		CHECK(blue < 3000);
		CHECK(blue > 0);
	}

	SECTION("Neighboring cells") {
		SECTION("Top") {
			SECTION("Exist") {
				size_t w = 1, h = 2;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto preset = board.GetCellByGlobalCoord(0,0);
				preset->color = cubes::Color::BLUE;
				auto cell = board.GetCellByGlobalCoord(0,1);
				auto target = board.GetTop(cell);
				REQUIRE_FALSE(target == nullptr);
				CHECK(target->color == cubes::Color::BLUE);
			}
			SECTION("Not exist") {
				size_t w = 1, h = 1;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto cell = board.GetCellByGlobalCoord(0,0);
				auto target = board.GetTop(cell);
				CHECK(target == nullptr);
			}
		}
		SECTION("Bottom") {
			SECTION("Exist") {
				size_t w = 1, h = 2;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto preset = board.GetCellByGlobalCoord(0,1);
				preset->color = cubes::Color::BLUE;
				auto cell = board.GetCellByGlobalCoord(0,0);
				auto target = board.GetBottom(cell);
				REQUIRE_FALSE(target == nullptr);
				CHECK(target->color == cubes::Color::BLUE);
			}
			SECTION("Not exist") {
				size_t w = 1, h = 1;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto cell = board.GetCellByGlobalCoord(0,0);
				auto target = board.GetBottom(cell);
				CHECK(target == nullptr);
			}
		}
		SECTION("Left") {
			SECTION("Exist") {
				size_t w = 2, h = 1;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto preset = board.GetCellByGlobalCoord(0,0);
				preset->color = cubes::Color::BLUE;
				auto cell = board.GetCellByGlobalCoord(1,0);
				auto target = board.GetLeft(cell);
				REQUIRE_FALSE(target == nullptr);
				CHECK(target->color == cubes::Color::BLUE);
			}
			SECTION("Not exist") {
				size_t w = 1, h = 1;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto cell = board.GetCellByGlobalCoord(0,0);
				auto target = board.GetLeft(cell);
				CHECK(target == nullptr);
			}
		}
		SECTION("Right") {
			SECTION("Exist") {
				size_t w = 2, h = 1;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto preset = board.GetCellByGlobalCoord(1,0);
				preset->color = cubes::Color::BLUE;
				auto cell = board.GetCellByGlobalCoord(0,0);
				auto target = board.GetRight(cell);
				REQUIRE_FALSE(target == nullptr);
				CHECK(target->color == cubes::Color::BLUE);
			}
			SECTION("Not exist") {
				size_t w = 1, h = 1;
				GameBoard board (w, h, {cubes::Color::YELLOW});
				auto cell = board.GetCellByGlobalCoord(0,0);
				auto target = board.GetRight(cell);
				CHECK(target == nullptr);
			}
		}
	}

	SECTION("Clobal coordinates") {
		SECTION("Cell size by default") {
			SECTION("Single cell") {
				size_t w = 1, h = 1;
				GameBoard board (w, h, {cubes::Color::RED});
				auto & field = board.GetField();
				auto cell = board.GetCellByGlobalCoord(0,0);
				REQUIRE(cell == &field.at(0));
			}
			SECTION("first cell") {
				size_t w = 2, h = 3;
				GameBoard board (w, h, {cubes::Color::RED});
				auto & field = board.GetField();
				auto cell = board.GetCellByGlobalCoord(0,0);
				REQUIRE(cell == &field.at(0));
			}
			SECTION("last cell") {
				size_t w = 2, h = 3;
				GameBoard board (w, h, {cubes::Color::RED});
				auto & field = board.GetField();
				auto cell = board.GetCellByGlobalCoord(1,2);
				REQUIRE(cell == &field.at(5));
			}
			SECTION("middle cell") {
				size_t w = 2, h = 3;
				GameBoard board (w, h, {cubes::Color::RED});
				auto & field = board.GetField();
				auto cell = board.GetCellByGlobalCoord(1,1);
				REQUIRE(cell == &field.at(3));
			}
		}
		SECTION("Cell with custom size") {
			SECTION("Single cell") {
				size_t w = 1, h = 1;
				GameBoard board (w, h, {cubes::Color::RED});
				board.SetCellSize(5);
				auto & field = board.GetField();
				auto cell = board.GetCellByGlobalCoord(3,3);
				REQUIRE(cell == &field.at(0));
			}
			SECTION("left top cell") {
				size_t w = 2, h = 2;
				GameBoard board (w, h, {cubes::Color::RED});
				board.SetCellSize(3);
				SECTION("left top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(0,0);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("left middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(0,1);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("left bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(0,2);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("middle top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(1,0);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("middle middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(1,1);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("middle bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(1,2);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("right top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(2,0);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("right middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(2,1);
					REQUIRE(cell == &field.at(0));
				}
				SECTION("right bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(2,2);
					REQUIRE(cell == &field.at(0));
				}
			}
			SECTION("right top cell") {
				size_t w = 2, h = 2;
				GameBoard board (w, h, {cubes::Color::RED});
				board.SetCellSize(3);
				SECTION("left top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(3,0);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("left middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(3,1);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("left bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(3,2);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("middle top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(4,0);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("middle middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(4,1);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("middle bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(4,2);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("right top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(5,0);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("right middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(5,1);
					REQUIRE(cell == &field.at(1));
				}
				SECTION("right bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(5,2);
					REQUIRE(cell == &field.at(1));
				}
			}
			SECTION("left bottom cell") {
				size_t w = 2, h = 2;
				GameBoard board (w, h, {cubes::Color::RED});
				board.SetCellSize(3);
				SECTION("left top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(0,3);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("left middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(0,4);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("left bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(0,5);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("middle top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(1,3);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("middle middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(1,4);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("middle bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(1,5);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("right top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(2,3);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("right middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(2,4);
					REQUIRE(cell == &field.at(2));
				}
				SECTION("right bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(2,5);
					REQUIRE(cell == &field.at(2));
				}
			}
			SECTION("right bottom cell") {
				size_t w = 2, h = 2;
				GameBoard board (w, h, {cubes::Color::RED});
				board.SetCellSize(3);
				SECTION("left top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(3,3);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("left middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(3,4);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("left bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(3,5);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("middle top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(4,3);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("middle middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(4,4);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("middle bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(4,5);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("right top pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(5,3);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("right middle pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(5,4);
					REQUIRE(cell == &field.at(3));
				}
				SECTION("right bottom pixel") {
					auto & field = board.GetField();
					auto cell = board.GetCellByGlobalCoord(5,5);
					REQUIRE(cell == &field.at(3));
				}
			}

		}
	}
	SECTION("Local coordinates") {
		SECTION("Zero coordinates") {
			size_t w = 2, h = 3;
			GameBoard board (w, h, {cubes::Color::RED});
			auto & field = board.GetField();
			auto cell = board.GetCellByLocalCoord({0,0});
			REQUIRE(cell == &field.at(0));
		}
		SECTION("Last coordinates") {
			size_t w = 2, h = 3;
			GameBoard board (w, h, {cubes::Color::RED});
			auto & field = board.GetField();
			auto cell = board.GetCellByLocalCoord({1,2});
			REQUIRE(cell == &field.at(5));
		}
		SECTION("Middle coordinates") {
			size_t w = 2, h = 3;
			GameBoard board (w, h, {cubes::Color::RED});
			auto & field = board.GetField();
			auto cell = board.GetCellByLocalCoord({1,1});
			REQUIRE(cell == &field.at(3));
		}
	}
}
