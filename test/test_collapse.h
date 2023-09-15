#include "collapse_game.h"

#include <numeric>

TEST_CASE("Collapse game") {
	using namespace collapse;
	using namespace cubes;

	SECTION("Init"){
		SECTION("Check setting bonuses"){
			SECTION("Zero values"){
				BonusContainer bonuses = {{Bonuses::REMOVE_CELL, 0}, {Bonuses::CHANGE_CELL_COLOR, 0}, {Bonuses::REMOVE_CELL, 0}};
				CollapseGame cg (4, 3, {Color::RED, Color::GREEN, Color::BLUE}, 4, bonuses);
				CHECK(cg.GetBonuses() == bonuses);
			}
			SECTION("Non-zero values"){
				BonusContainer bonuses = {{Bonuses::REMOVE_CELL, 1}, {Bonuses::CHANGE_CELL_COLOR, 2}, {Bonuses::REMOVE_CELL, 3}};
				CollapseGame cg (4, 3, {Color::RED, Color::GREEN, Color::BLUE},
					4, bonuses);
				CHECK(cg.GetBonuses() == bonuses);
			}
		}
		SECTION("Check setting board-size") {
			BonusContainer bonuses = {{Bonuses::REMOVE_CELL, 1}, {Bonuses::CHANGE_CELL_COLOR, 2}, {Bonuses::REMOVE_CELL, 3}};
			CollapseGame cg (4, 3, {Color::RED}, 4, bonuses);
			auto board = cg.GetGameBoard();
			CHECK(board.size() == 4 * 3);
			CHECK(std::accumulate(board.begin(), board.end(), 0,
				[](int sum, Cell cell){
					return sum + static_cast<int>(cell.color == Color::RED);
				}) == 4 * 3);
			CHECK(std::accumulate(board.begin(), board.end(), 0,
				[](int sum, Cell cell){
					return sum + static_cast<int>(cell.status == Status::NORMAL);
				}) == 4 * 3);
		}
	}
	SECTION("Hover") {
		SECTION("Single block must be no hover") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			// пиксели ячейки 5 :
			//    2 3
			// 2  * *
			// 3  Х *
			cg.SetHover(2, 3);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("All block must be hovered") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			cg.SetHover(0, 0);
			CHECK(board->at(0).status == Status::ACTIVE);
			CHECK(board->at(1).status == Status::ACTIVE);
			CHECK(board->at(2).status == Status::ACTIVE);
			CHECK(board->at(3).status == Status::ACTIVE);

			CHECK(board->at(4).status == Status::ACTIVE);
			CHECK(board->at(5).status == Status::ACTIVE);
			CHECK(board->at(6).status == Status::ACTIVE);
			CHECK(board->at(7).status == Status::ACTIVE);

			CHECK(board->at(8).status == Status::ACTIVE);
			CHECK(board->at(9).status == Status::ACTIVE);
			CHECK(board->at(10).status == Status::ACTIVE);
			CHECK(board->at(11).status == Status::ACTIVE);
		}
		SECTION("Hover to right") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			board->at(6).color = Color::GREEN;
			// пиксели ячейки 5 :
			//    2 3
			// 2  * *
			// 3  Х *
			cg.SetHover(2, 3);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::ACTIVE);
			CHECK(board->at(6).status == Status::ACTIVE);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Hover to left") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			board->at(6).color = Color::GREEN;
			// пиксели ячейки 5 :
			//    4 5
			// 2  X *
			// 3  * *
			cg.SetHover(4, 2);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::ACTIVE);
			CHECK(board->at(6).status == Status::ACTIVE);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Hover to bottom") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			board->at(9).color = Color::GREEN;
			// пиксели ячейки 5 :
			//    2 3
			// 2  * *
			// 3  X *
			cg.SetHover(2, 3);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::ACTIVE);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::ACTIVE);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Hover to top") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			board->at(1).color = Color::GREEN;
			// пиксели ячейки 5 :
			//    2 3
			// 2  * *
			// 3  X *
			cg.SetHover(2, 3);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::ACTIVE);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::ACTIVE);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("No hover on corners") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			board->at(0).color = Color::GREEN;
			board->at(2).color = Color::GREEN;
			board->at(8).color = Color::GREEN;
			board->at(10).color = Color::GREEN;
			// пиксели ячейки 5 :
			//    2 3
			// 2  * *
			// 3  X *
			cg.SetHover(2, 3);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Letter O hovered outer") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;
			board->at(6).color = Color::GREEN;

			cg.SetHover(0, 0);
			CHECK(board->at(0).status == Status::ACTIVE);
			CHECK(board->at(1).status == Status::ACTIVE);
			CHECK(board->at(2).status == Status::ACTIVE);
			CHECK(board->at(3).status == Status::ACTIVE);

			CHECK(board->at(4).status == Status::ACTIVE);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::ACTIVE);

			CHECK(board->at(8).status == Status::ACTIVE);
			CHECK(board->at(9).status == Status::ACTIVE);
			CHECK(board->at(10).status == Status::ACTIVE);
			CHECK(board->at(11).status == Status::ACTIVE);
		}
		SECTION("Hover empty has not effect") {
			CollapseGame cg (4, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).isEmpty = true;
			board->at(6).isEmpty = true;

			cg.SetHover(1, 1);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("SetHover return score (pow of amount)") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CLEAR_SAME_COLOR, 1}});
			CHECK(cg.SetHover(0, 0) == (4 * 4));
		}
		SECTION("SetHover return 0 if empty cell") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CLEAR_SAME_COLOR, 1}});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).isEmpty = true;
			CHECK(cg.SetHover(0, 0) == 0);
		}
		SECTION("SetHover return 0 if single cell") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CLEAR_SAME_COLOR, 1}});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).color = Color::SKY;
			CHECK(cg.SetHover(0, 0) == 0);
		}
		SECTION("SetHover clear prev hover") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1
			// 2  3
			board->at(0).color = Color::GREEN;
			board->at(1).color = Color::GREEN;

			cg.SetHover(0, 0);
			CHECK(board->at(0).status == Status::ACTIVE);
			CHECK(board->at(1).status == Status::ACTIVE);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			cg.SetHover(0, 1);
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::ACTIVE);
			CHECK(board->at(3).status == Status::ACTIVE);
		}
	}
	SECTION("Clear hover") {
		CollapseGame cg (4, 3, {Color::RED}, 2, {});
		std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
		cg.SetHover(0, 0);
		cg.ClearHover();
		CHECK(board->at(0).status == Status::NORMAL);
		CHECK(board->at(1).status == Status::NORMAL);
		CHECK(board->at(2).status == Status::NORMAL);
		CHECK(board->at(3).status == Status::NORMAL);

		CHECK(board->at(4).status == Status::NORMAL);
		CHECK(board->at(5).status == Status::NORMAL);
		CHECK(board->at(6).status == Status::NORMAL);
		CHECK(board->at(7).status == Status::NORMAL);

		CHECK(board->at(8).status == Status::NORMAL);
		CHECK(board->at(9).status == Status::NORMAL);
		CHECK(board->at(10).status == Status::NORMAL);
		CHECK(board->at(11).status == Status::NORMAL);
	}
	SECTION("Hover hint") {
		SECTION("No hints") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::WHITE;
			board->at(1).color = Color::BLACK;
			board->at(2).color = Color::RED;
			board->at(3).color = Color::BLUE;
			board->at(4).color = Color::YELLOW;
			board->at(5).color = Color::ORANGE;
			board->at(6).color = Color::GREEN;
			board->at(7).color = Color::VIOLET;
			board->at(8).color = Color::PINK;
			board->at(9).color = Color::SKY;
			board->at(10).color = Color::LIME;
			board->at(11).color = Color::BROWN;

			CHECK_FALSE(cg.HoverHint());
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Single hints in begin of field") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::WHITE;
			board->at(1).color = Color::WHITE;
			board->at(2).color = Color::RED;
			board->at(3).color = Color::BLUE;
			board->at(4).color = Color::WHITE;
			board->at(5).color = Color::ORANGE;
			board->at(6).color = Color::GREEN;
			board->at(7).color = Color::VIOLET;
			board->at(8).color = Color::PINK;
			board->at(9).color = Color::SKY;
			board->at(10).color = Color::LIME;
			board->at(11).color = Color::BROWN;

			CHECK(cg.HoverHint());
			CHECK(board->at(0).status == Status::ACTIVE);
			CHECK(board->at(1).status == Status::ACTIVE);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::ACTIVE);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Single hints in middle of field") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::WHITE;
			board->at(1).color = Color::BLACK;
			board->at(2).color = Color::RED;
			board->at(3).color = Color::BLUE;
			board->at(4).color = Color::YELLOW;
			board->at(5).color = Color::ORANGE;
			board->at(6).color = Color::RED;
			board->at(7).color = Color::VIOLET;
			board->at(8).color = Color::PINK;
			board->at(9).color = Color::SKY;
			board->at(10).color = Color::LIME;
			board->at(11).color = Color::BROWN;

			CHECK(cg.HoverHint());
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::ACTIVE);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::ACTIVE);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Single hints in end of field") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::WHITE;
			board->at(1).color = Color::BLACK;
			board->at(2).color = Color::RED;
			board->at(3).color = Color::BLUE;
			board->at(4).color = Color::YELLOW;
			board->at(5).color = Color::ORANGE;
			board->at(6).color = Color::LIME;
			board->at(7).color = Color::VIOLET;
			board->at(8).color = Color::PINK;
			board->at(9).color = Color::SKY;
			board->at(10).color = Color::BROWN;
			board->at(11).color = Color::BROWN;

			CHECK(cg.HoverHint());
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::ACTIVE);
			CHECK(board->at(11).status == Status::ACTIVE);
		}
		SECTION("Biggest hints before other") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::WHITE;
			board->at(1).color = Color::WHITE;
			board->at(2).color = Color::WHITE;
			board->at(3).color = Color::BLUE;
			board->at(4).color = Color::YELLOW;
			board->at(5).color = Color::ORANGE;
			board->at(6).color = Color::LIME;
			board->at(7).color = Color::VIOLET;
			board->at(8).color = Color::PINK;
			board->at(9).color = Color::SKY;
			board->at(10).color = Color::LIME;
			board->at(11).color = Color::BROWN;

			CHECK(cg.HoverHint());
			CHECK(board->at(0).status == Status::ACTIVE);
			CHECK(board->at(1).status == Status::ACTIVE);
			CHECK(board->at(2).status == Status::ACTIVE);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Biggest hints after other") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::WHITE;
			board->at(1).color = Color::WHITE;
			board->at(2).color = Color::LIME;
			board->at(3).color = Color::BLUE;
			board->at(4).color = Color::YELLOW;
			board->at(5).color = Color::ORANGE;
			board->at(6).color = Color::LIME;
			board->at(7).color = Color::VIOLET;
			board->at(8).color = Color::PINK;
			board->at(9).color = Color::SKY;
			board->at(10).color = Color::LIME;
			board->at(11).color = Color::BROWN;

			CHECK(cg.HoverHint());
			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::ACTIVE);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::ACTIVE);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::ACTIVE);
			CHECK(board->at(11).status == Status::NORMAL);
		}
	}
	SECTION("Bonus hover") {
		SECTION("Single cell") {
			SECTION("Clear prev hover and set new") {
				CollapseGame cg (2, 2, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1
				// 2  3
				board->at(0).color = Color::GREEN;
				board->at(1).color = Color::GREEN;

				cg.SetHover(0, 0);
				CHECK(board->at(0).status == Status::ACTIVE);
				CHECK(board->at(1).status == Status::ACTIVE);
				CHECK(board->at(2).status == Status::NORMAL);
				CHECK(board->at(3).status == Status::NORMAL);

				cg.SetBonusHover(0, 1);
				CHECK(board->at(0).status == Status::NORMAL);
				CHECK(board->at(1).status == Status::NORMAL);
				CHECK(board->at(2).status == Status::ACTIVE);
				CHECK(board->at(3).status == Status::NORMAL);
			}
		}
		SECTION("All") {
			SECTION("Clear prev hover and set new") {
				CollapseGame cg (2, 2, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1
				// 2  3
				board->at(0).color = Color::GREEN;
				board->at(1).color = Color::GREEN;

				cg.SetHover(0, 0);
				CHECK(board->at(0).status == Status::ACTIVE);
				CHECK(board->at(1).status == Status::ACTIVE);
				CHECK(board->at(2).status == Status::NORMAL);
				CHECK(board->at(3).status == Status::NORMAL);

				cg.SetBonusHover(Color::RED);
				CHECK(board->at(0).status == Status::NORMAL);
				CHECK(board->at(1).status == Status::NORMAL);
				CHECK(board->at(2).status == Status::ACTIVE);
				CHECK(board->at(3).status == Status::ACTIVE);
			}
		}
	}
	SECTION("Collapse") {
		SECTION("Clear Active-status after collapse") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(1).color = Color::GREEN;
			board->at(2).color = Color::GREEN;

			cg.Collapse(2, 0);

			CHECK(board->at(0).status == Status::NORMAL);
			CHECK(board->at(1).status == Status::NORMAL);
			CHECK(board->at(2).status == Status::NORMAL);
			CHECK(board->at(3).status == Status::NORMAL);

			CHECK(board->at(4).status == Status::NORMAL);
			CHECK(board->at(5).status == Status::NORMAL);
			CHECK(board->at(6).status == Status::NORMAL);
			CHECK(board->at(7).status == Status::NORMAL);

			CHECK(board->at(8).status == Status::NORMAL);
			CHECK(board->at(9).status == Status::NORMAL);
			CHECK(board->at(10).status == Status::NORMAL);
			CHECK(board->at(11).status == Status::NORMAL);
		}
		SECTION("Collapse in top must not move other cells") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(1).color = Color::GREEN;
			board->at(2).color = Color::GREEN;

			cg.Collapse(2, 0);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);

			CHECK(board->at(0).color == Color::RED);
			CHECK(board->at(3).color == Color::RED);

			CHECK(board->at(4).color == Color::RED);
			CHECK(board->at(5).color == Color::RED);
			CHECK(board->at(6).color == Color::RED);
			CHECK(board->at(7).color == Color::RED);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::RED);
			CHECK(board->at(11).color == Color::RED);
		}
		SECTION("Collapse in middle move cells to down") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(1).color = Color::BLUE;
			board->at(2).color = Color::BLUE;
			board->at(5).color = Color::GREEN;
			board->at(6).color = Color::GREEN;

			cg.Collapse(3, 3);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);

			CHECK(board->at(0).color == Color::RED);
			CHECK(board->at(3).color == Color::RED);

			CHECK(board->at(4).color == Color::RED);
			CHECK(board->at(5).color == Color::BLUE);
			CHECK(board->at(6).color == Color::BLUE);
			CHECK(board->at(7).color == Color::RED);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::RED);
			CHECK(board->at(11).color == Color::RED);
		}
		SECTION("Collapse in bottom move cells to down") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::BLUE;
			board->at(6).color = Color::BLUE;
			board->at(9).color = Color::GREEN;
			board->at(10).color = Color::GREEN;

			cg.Collapse(3, 5);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);

			CHECK(board->at(0).color == Color::RED);
			CHECK(board->at(3).color == Color::RED);

			CHECK(board->at(4).color == Color::RED);
			CHECK(board->at(5).color == Color::RED);
			CHECK(board->at(6).color == Color::RED);
			CHECK(board->at(7).color == Color::RED);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::BLUE);
			CHECK(board->at(10).color == Color::BLUE);
			CHECK(board->at(11).color == Color::RED);
		}
		SECTION("Collapse in last col must not move other cells") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(3).color = Color::GREEN;
			board->at(7).color = Color::GREEN;
			board->at(11).color = Color::GREEN;

			cg.Collapse(7, 0);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK_FALSE(board->at(1).isEmpty);
			CHECK_FALSE(board->at(2).isEmpty);
			CHECK(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK(board->at(11).isEmpty);

			CHECK(board->at(0).color == Color::RED);
			CHECK(board->at(1).color == Color::RED);
			CHECK(board->at(2).color == Color::RED);

			CHECK(board->at(4).color == Color::RED);
			CHECK(board->at(5).color == Color::RED);
			CHECK(board->at(6).color == Color::RED);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::RED);
		}
		SECTION("Collapse in middle col move other cells to left") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(2).color = Color::GREEN;
			board->at(6).color = Color::GREEN;
			board->at(10).color = Color::GREEN;
			board->at(3).color = Color::BLUE;
			board->at(7).color = Color::BLUE;
			board->at(11).color = Color::BLUE;

			cg.Collapse(5, 0);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK_FALSE(board->at(1).isEmpty);
			CHECK_FALSE(board->at(2).isEmpty);
			CHECK(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK(board->at(11).isEmpty);

			CHECK(board->at(0).color == Color::RED);
			CHECK(board->at(1).color == Color::RED);
			CHECK(board->at(2).color == Color::BLUE);

			CHECK(board->at(4).color == Color::RED);
			CHECK(board->at(5).color == Color::RED);
			CHECK(board->at(6).color == Color::BLUE);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::BLUE);
		}
		SECTION("Collapse in left col move other cells to left") {
			CollapseGame cg (4, 3, {Color::RED}, 2, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(0).color = Color::GREEN;
			board->at(4).color = Color::GREEN;
			board->at(8).color = Color::GREEN;
			board->at(1).color = Color::BLUE;
			board->at(5).color = Color::BLUE;
			board->at(9).color = Color::BLUE;

			cg.Collapse(0, 0);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK_FALSE(board->at(1).isEmpty);
			CHECK_FALSE(board->at(2).isEmpty);
			CHECK(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK(board->at(11).isEmpty);

			CHECK(board->at(0).color == Color::BLUE);
			CHECK(board->at(1).color == Color::RED);
			CHECK(board->at(2).color == Color::RED);

			CHECK(board->at(4).color == Color::BLUE);
			CHECK(board->at(5).color == Color::RED);
			CHECK(board->at(6).color == Color::RED);

			CHECK(board->at(8).color == Color::BLUE);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::RED);
		}
		SECTION("Collapse complex figure") {
			CollapseGame cg (5, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			//  0  1  2  3  4
			//  5  6  7  8  9
			// 10 11 12 13 14
			board->at(2).color = Color::GREEN;
			board->at(1).color = Color::GREEN;
			board->at(6).color = Color::GREEN;
			board->at(11).color = Color::GREEN;
			board->at(12).color = Color::GREEN;
			board->at(13).color = Color::GREEN;
			board->at(8).color = Color::GREEN;

			board->at(7).color = Color::BLUE;

			board->at(3).color = Color::PINK;

			board->at(4).color = Color::LIME;
			board->at(9).color = Color::LIME;
			board->at(14).color = Color::LIME;

			cg.Collapse(1, 2);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);
			CHECK(board->at(4).isEmpty);

			CHECK_FALSE(board->at(5).isEmpty);
			CHECK(board->at(6).isEmpty);
			CHECK(board->at(7).isEmpty);
			CHECK_FALSE(board->at(8).isEmpty);
			CHECK(board->at(9).isEmpty);

			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);
			CHECK_FALSE(board->at(12).isEmpty);
			CHECK_FALSE(board->at(13).isEmpty);
			CHECK(board->at(14).isEmpty);

			CHECK(board->at(0).color == Color::RED);
			CHECK(board->at(3).color == Color::LIME);

			CHECK(board->at(5).color == Color::RED);
			CHECK(board->at(8).color == Color::LIME);

			CHECK(board->at(10).color == Color::RED);
			CHECK(board->at(11).color == Color::BLUE);
			CHECK(board->at(12).color == Color::PINK);
			CHECK(board->at(13).color == Color::LIME);
		}
		SECTION("Collapse single has no effect") {
			CollapseGame cg (4, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(5).color = Color::GREEN;

			cg.Collapse(1, 1);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK_FALSE(board->at(1).isEmpty);
			CHECK_FALSE(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);
		}
		SECTION("Collapse empty has no effect") {
			CollapseGame cg (4, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11
			board->at(1).isEmpty = true;

			cg.Collapse(1, 0);

			CHECK_FALSE(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK_FALSE(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);

			CHECK_FALSE(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);
		}
		SECTION("Multiple collapses"){
			CollapseGame cg (4, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11

			// 3 green cell
			board->at(1).color = Color::GREEN;
			board->at(2).color = Color::GREEN;
			board->at(3).color = Color::GREEN;

			// 2 lime cell
			board->at(0).color = Color::LIME;
			board->at(4).color = Color::LIME;

			// 3 blue cell
			board->at(5).color = Color::BLUE;
			board->at(6).color = Color::BLUE;
			board->at(7).color = Color::BLUE;

			// and 4 (8, 9, 10, 11) still red cell

			cg.Collapse(0, 0); // Lime collapse, nothing move

			CHECK(board->at(0).isEmpty);
			CHECK_FALSE(board->at(1).isEmpty);
			CHECK_FALSE(board->at(2).isEmpty);
			CHECK_FALSE(board->at(3).isEmpty);

			CHECK(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);

			CHECK(board->at(1).color == Color::GREEN);
			CHECK(board->at(2).color == Color::GREEN);
			CHECK(board->at(3).color == Color::GREEN);

			CHECK(board->at(5).color == Color::BLUE);
			CHECK(board->at(6).color == Color::BLUE);
			CHECK(board->at(7).color == Color::BLUE);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::RED);
			CHECK(board->at(11).color == Color::RED);

			cg.Collapse(1, 1); // blue collapse, green move down

			CHECK(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK(board->at(3).isEmpty);

			CHECK(board->at(4).isEmpty);
			CHECK_FALSE(board->at(5).isEmpty);
			CHECK_FALSE(board->at(6).isEmpty);
			CHECK_FALSE(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK_FALSE(board->at(11).isEmpty);

			CHECK(board->at(5).color == Color::GREEN);
			CHECK(board->at(6).color == Color::GREEN);
			CHECK(board->at(7).color == Color::GREEN);

			CHECK(board->at(8).color == Color::RED);
			CHECK(board->at(9).color == Color::RED);
			CHECK(board->at(10).color == Color::RED);
			CHECK(board->at(11).color == Color::RED);

			cg.Collapse(0, 2); // Red collapse, green move down

			CHECK(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK(board->at(3).isEmpty);

			CHECK(board->at(4).isEmpty);
			CHECK(board->at(5).isEmpty);
			CHECK(board->at(6).isEmpty);
			CHECK(board->at(7).isEmpty);

			CHECK_FALSE(board->at(8).isEmpty);
			CHECK_FALSE(board->at(9).isEmpty);
			CHECK_FALSE(board->at(10).isEmpty);
			CHECK(board->at(11).isEmpty);

			CHECK(board->at(8).color == Color::GREEN);
			CHECK(board->at(9).color == Color::GREEN);
			CHECK(board->at(10).color == Color::GREEN);

			cg.Collapse(0, 2); // Green collapse

			CHECK(board->at(0).isEmpty);
			CHECK(board->at(1).isEmpty);
			CHECK(board->at(2).isEmpty);
			CHECK(board->at(3).isEmpty);

			CHECK(board->at(4).isEmpty);
			CHECK(board->at(5).isEmpty);
			CHECK(board->at(6).isEmpty);
			CHECK(board->at(7).isEmpty);

			CHECK(board->at(8).isEmpty);
			CHECK(board->at(9).isEmpty);
			CHECK(board->at(10).isEmpty);
			CHECK(board->at(11).isEmpty);
		}
	}
	SECTION("Bonuses") {
		SECTION("Change Color") {
			SECTION("Has bonus") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {{Bonuses::CHANGE_CELL_COLOR, 2}});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				cg.ApplyBonusChangeColor(1,1,Color::SKY);

				CHECK(board->at(0).color == Color::RED);
				CHECK(board->at(1).color == Color::RED);
				CHECK(board->at(2).color == Color::RED);

				CHECK(board->at(3).color == Color::RED);
				CHECK(board->at(4).color == Color::SKY);
				CHECK(board->at(5).color == Color::RED);

				CHECK(board->at(6).color == Color::RED);
				CHECK(board->at(7).color == Color::RED);
				CHECK(board->at(8).color == Color::RED);

				CHECK(cg.GetBonuses().at(Bonuses::CHANGE_CELL_COLOR) == 1);

				cg.ApplyBonusChangeColor(1,1,Color::GREEN);
				CHECK(cg.GetBonuses().at(Bonuses::CHANGE_CELL_COLOR) == 0);
			}
			SECTION("has no bonuses") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				cg.ApplyBonusChangeColor(1,1,Color::SKY);

				CHECK(board->at(0).color == Color::RED);
				CHECK(board->at(1).color == Color::RED);
				CHECK(board->at(2).color == Color::RED);

				CHECK(board->at(3).color == Color::RED);
				CHECK(board->at(4).color == Color::RED);
				CHECK(board->at(5).color == Color::RED);

				CHECK(board->at(6).color == Color::RED);
				CHECK(board->at(7).color == Color::RED);
				CHECK(board->at(8).color == Color::RED);

				CHECK(cg.GetBonuses().at(Bonuses::CHANGE_CELL_COLOR) == 0);
			}
		}
		SECTION("Remove cell") {
			SECTION("Has bonus and same color around (multiple test)") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {{Bonuses::REMOVE_CELL, 2}});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				cg.ApplyBonusRemoveCell(1,1);

				CHECK(board->at(0).color == Color::RED);
				CHECK(board->at(2).color == Color::RED);

				CHECK(board->at(3).color == Color::RED);
				CHECK(board->at(4).color == Color::RED);
				CHECK(board->at(5).color == Color::RED);

				CHECK(board->at(6).color == Color::RED);
				CHECK(board->at(7).color == Color::RED);
				CHECK(board->at(8).color == Color::RED);

				CHECK_FALSE(board->at(0).isEmpty);
				CHECK(board->at(1).isEmpty);
				CHECK_FALSE(board->at(2).isEmpty);

				CHECK_FALSE(board->at(3).isEmpty);
				CHECK_FALSE(board->at(4).isEmpty);
				CHECK_FALSE(board->at(5).isEmpty);

				CHECK_FALSE(board->at(6).isEmpty);
				CHECK_FALSE(board->at(7).isEmpty);
				CHECK_FALSE(board->at(8).isEmpty);

				CHECK(cg.GetBonuses().at(Bonuses::REMOVE_CELL) == 1);

				cg.ApplyBonusRemoveCell(1,1);

				CHECK_FALSE(board->at(0).isEmpty);
				CHECK(board->at(1).isEmpty);
				CHECK_FALSE(board->at(2).isEmpty);

				CHECK_FALSE(board->at(3).isEmpty);
				CHECK(board->at(4).isEmpty);
				CHECK_FALSE(board->at(5).isEmpty);

				CHECK_FALSE(board->at(6).isEmpty);
				CHECK_FALSE(board->at(7).isEmpty);
				CHECK_FALSE(board->at(8).isEmpty);

				CHECK(cg.GetBonuses().at(Bonuses::REMOVE_CELL) == 0);
			}
			SECTION("Has bonus on different color around") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {{Bonuses::REMOVE_CELL, 1}});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				board->at(4).color = Color::SKY;
				cg.ApplyBonusRemoveCell(1,1);


				CHECK(board->at(0).color == Color::RED);
				CHECK(board->at(2).color == Color::RED);

				CHECK(board->at(3).color == Color::RED);
				CHECK(board->at(4).color == Color::RED);
				CHECK(board->at(5).color == Color::RED);

				CHECK(board->at(6).color == Color::RED);
				CHECK(board->at(7).color == Color::RED);
				CHECK(board->at(8).color == Color::RED);

				CHECK_FALSE(board->at(0).isEmpty);
				CHECK(board->at(1).isEmpty);
				CHECK_FALSE(board->at(2).isEmpty);

				CHECK_FALSE(board->at(3).isEmpty);
				CHECK_FALSE(board->at(4).isEmpty);
				CHECK_FALSE(board->at(5).isEmpty);

				CHECK_FALSE(board->at(6).isEmpty);
				CHECK_FALSE(board->at(7).isEmpty);
				CHECK_FALSE(board->at(8).isEmpty);

				CHECK(cg.GetBonuses().at(Bonuses::REMOVE_CELL) == 0);
			}
			SECTION("has no bonuses") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				cg.ApplyBonusRemoveCell(1,1);

				CHECK_FALSE(board->at(0).isEmpty);
				CHECK_FALSE(board->at(1).isEmpty);
				CHECK_FALSE(board->at(2).isEmpty);

				CHECK_FALSE(board->at(3).isEmpty);
				CHECK_FALSE(board->at(4).isEmpty);
				CHECK_FALSE(board->at(5).isEmpty);

				CHECK_FALSE(board->at(6).isEmpty);
				CHECK_FALSE(board->at(7).isEmpty);
				CHECK_FALSE(board->at(8).isEmpty);

				CHECK(cg.GetBonuses().at(Bonuses::REMOVE_CELL) == 0);
			}
		}
		SECTION("Clear color (remove all)") {
			SECTION("Has bonus") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {{Bonuses::CLEAR_SAME_COLOR, 2}});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				board->at(1).color = Color::SKY;
				board->at(3).color = Color::SKY;
				board->at(5).color = Color::SKY;
				board->at(7).color = Color::SKY;

				cg.ApplyBonusClearColor(Color::SKY);

				CHECK(board->at(0).isEmpty);
				CHECK(board->at(1).isEmpty);
				CHECK(board->at(2).isEmpty);

				CHECK_FALSE(board->at(3).isEmpty);
				CHECK(board->at(4).isEmpty);
				CHECK_FALSE(board->at(5).isEmpty);

				CHECK_FALSE(board->at(6).isEmpty);
				CHECK_FALSE(board->at(7).isEmpty);
				CHECK_FALSE(board->at(8).isEmpty);

				CHECK(board->at(3).color == Color::RED);
				CHECK(board->at(5).color == Color::RED);

				CHECK(board->at(6).color == Color::RED);
				CHECK(board->at(7).color == Color::RED);
				CHECK(board->at(8).color == Color::RED);

				CHECK(cg.GetBonuses().at(Bonuses::CLEAR_SAME_COLOR) == 1);

				cg.ApplyBonusClearColor(Color::RED);
				CHECK(cg.GetBonuses().at(Bonuses::CLEAR_SAME_COLOR) == 0);

				CHECK(board->at(0).isEmpty);
				CHECK(board->at(1).isEmpty);
				CHECK(board->at(2).isEmpty);

				CHECK(board->at(3).isEmpty);
				CHECK(board->at(4).isEmpty);
				CHECK(board->at(5).isEmpty);

				CHECK(board->at(6).isEmpty);
				CHECK(board->at(7).isEmpty);
				CHECK(board->at(8).isEmpty);
			}
			SECTION("has no bonuses") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				cg.ApplyBonusClearColor(Color::RED);

				CHECK_FALSE(board->at(0).isEmpty);
				CHECK_FALSE(board->at(1).isEmpty);
				CHECK_FALSE(board->at(2).isEmpty);

				CHECK_FALSE(board->at(3).isEmpty);
				CHECK_FALSE(board->at(4).isEmpty);
				CHECK_FALSE(board->at(5).isEmpty);

				CHECK_FALSE(board->at(6).isEmpty);
				CHECK_FALSE(board->at(7).isEmpty);
				CHECK_FALSE(board->at(8).isEmpty);

				CHECK(cg.GetBonuses().at(Bonuses::CLEAR_SAME_COLOR) == 0);
			}
		}
	}
	SECTION("Game status") {
		SECTION("isGameOver") {
			SECTION("Has cells to collapse") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {});

				auto gameOver = cg.GetGameStatus();
				CHECK_FALSE(gameOver.isGameOver);
			}
			SECTION("Nothing to collapse") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				// indexes
				// 0  1  2
				// 3  4  5
				// 6  7  8
				board->at(1).color = Color::SKY;
				board->at(3).color = Color::SKY;
				board->at(5).color = Color::SKY;
				board->at(7).color = Color::SKY;

				auto gameOver = cg.GetGameStatus();
				CHECK(gameOver.isGameOver);
			}
		}
		SECTION("isWin") {
			SECTION("Has cells in the board") {
				CollapseGame cg (3, 3, {Color::RED}, 1, {});

				auto gameOver = cg.GetGameStatus();
				CHECK_FALSE(gameOver.isWin);
			}
			SECTION("Nothing to collapse") {
				CollapseGame cg (2, 2, {Color::RED}, 1, {});
				std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
				board->at(0).isEmpty = true;
				board->at(1).isEmpty = true;
				board->at(2).isEmpty = true;
				board->at(3).isEmpty = true;

				auto gameOver = cg.GetGameStatus();
				CHECK(gameOver.isWin);
			}
		}
	}
	SECTION("Colors in game") {
		SECTION("Single color at start of game") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			auto colors = cg.GetColorsInGame();
			CHECK(colors.at(Color::RED) == 4);
		}
		SECTION("Multiple color at start of game") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).color = Color::SKY;
			board->at(1).color = Color::SKY;
			auto colors = cg.GetColorsInGame();
			CHECK(colors.at(Color::RED) == 2);
			CHECK(colors.at(Color::SKY) == 2);
		}
		SECTION("Some sells is empty") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).color = Color::SKY;
			board->at(0).isEmpty = true;
			board->at(1).color = Color::SKY;
			auto colors = cg.GetColorsInGame();
			CHECK(colors.at(Color::RED) == 2);
			CHECK(colors.at(Color::SKY) == 1);
		}
		SECTION("Empty field") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).isEmpty = true;
			board->at(1).isEmpty = true;
			board->at(2).isEmpty = true;
			board->at(3).isEmpty = true;
			auto colors = cg.GetColorsInGame();
			CHECK(colors.size() == 0);
		}
	}
	SECTION("Game statistic") {
		SECTION("At start of game") {
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			CHECK(cg.GetColorStatistic().size() == 0);
		}
		SECTION("After some collapses"){
			CollapseGame cg (4, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11

			// 3 green cell
			board->at(1).color = Color::GREEN;
			board->at(2).color = Color::GREEN;
			board->at(3).color = Color::GREEN;

			// 2 lime cell
			board->at(0).color = Color::LIME;
			board->at(4).color = Color::LIME;

			// 3 blue cell
			board->at(5).color = Color::BLUE;
			board->at(6).color = Color::BLUE;
			board->at(7).color = Color::BLUE;

			// and 4 still red cell

			cg.Collapse(1, 0); // GREEN score = 3 * 3
			cg.Collapse(0, 0); // LIME score = 2 * 2
			cg.Collapse(0, 2); // RED score = 4 * 4

			auto statistic = cg.GetColorStatistic();
			REQUIRE(statistic.size() == 3); // за игру схлопнуто 3 цвета
			CHECK(statistic.at(Color::LIME) == (2 * 2));
			CHECK(statistic.at(Color::GREEN) == (3 * 3));
			CHECK(statistic.at(Color::RED) == (4 * 4));
		}
		SECTION("After some collapses"){
			CollapseGame cg (4, 3, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			// indexes
			// 0  1  2  3
			// 4  5  6  7
			// 8  9 10 11

			// 3 green cell
			board->at(1).color = Color::GREEN;
			board->at(2).color = Color::GREEN;
			board->at(3).color = Color::GREEN;

			// 2 lime cell
			board->at(0).color = Color::LIME;
			board->at(4).color = Color::LIME;

			// 3 blue cell
			board->at(5).color = Color::BLUE;
			board->at(6).color = Color::BLUE;
			board->at(7).color = Color::BLUE;

			// and 4 still red cell

			cg.Collapse(1, 0); // GREEN score = 3 * 3
			cg.Collapse(0, 0); // LIME score = 2 * 2
			cg.Collapse(0, 2); // RED score = 4 * 4

			auto statistic = cg.GetColorStatistic();
			REQUIRE(statistic.size() == 3); // за игру схлопнуто 3 цвета
			CHECK(statistic.at(Color::LIME) == (2 * 2));
			CHECK(statistic.at(Color::GREEN) == (3 * 3));
			CHECK(statistic.at(Color::RED) == (4 * 4));
		}
		SECTION("Change color bonus has no effect on statistic"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CHANGE_CELL_COLOR, 1}});
			auto startStatistic = cg.GetColorStatistic();
			cg.ApplyBonusChangeColor(0, 0, Color::SKY);
			CHECK(cg.GetColorStatistic() == startStatistic);
		}
		SECTION("Remove single cell bonus has no effect on statistic"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::REMOVE_CELL, 1}});
			auto startStatistic = cg.GetColorStatistic();
			cg.ApplyBonusRemoveCell(0, 0);
			CHECK(cg.GetColorStatistic() == startStatistic);
		}
		SECTION("Clear all same color cells bonus"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CLEAR_SAME_COLOR, 1}});
			cg.ApplyBonusClearColor(Color::RED);
			auto statistic = cg.GetColorStatistic();
			REQUIRE(statistic.size() == 1);
			CHECK(statistic.at(Color::RED) == (4 * 4)); // счет равен квадрату количества
		}

	}
	SECTION("History and cancel turn") {
		SECTION("History stack at collapse"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).color = Color::SKY;
			board->at(1).color = Color::SKY;

			REQUIRE_FALSE(cg.HasCancelHistory());
			auto fieldStep0 = cg.GetGameBoard();

			cg.Collapse(0, 0);
			CHECK(cg.HasCancelHistory());
			auto fieldStep1 = cg.GetGameBoard();

			cg.Collapse(0, 1);
			CHECK(cg.HasCancelHistory());
			CHECK_FALSE(cg.GetGameBoard() == fieldStep1);

			cg.CancelTurn();
			CHECK(cg.HasCancelHistory());
			CHECK(cg.GetGameBoard() == fieldStep1);

			cg.CancelTurn();
			CHECK_FALSE(cg.HasCancelHistory());
			CHECK(cg.GetGameBoard() == fieldStep0);
		}
		SECTION("Cancel turn return statistic"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {});
			std::vector<Cell> * board = const_cast<std::vector<Cell>*>(&cg.GetGameBoard());
			board->at(0).color = Color::SKY;
			board->at(1).color = Color::SKY;
			auto startStatistic = cg.GetColorStatistic();
			cg.Collapse(0, 0);
			auto nextStatistic = cg.GetColorStatistic();
			REQUIRE_FALSE(startStatistic == nextStatistic);
			cg.CancelTurn();
			auto cancelStatistic = cg.GetColorStatistic();
			CHECK(startStatistic == cancelStatistic);
		}
		SECTION("History work with ClearColor bonus"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CLEAR_SAME_COLOR, 1}});

			REQUIRE_FALSE(cg.HasCancelHistory());
			auto startBonuses = cg.GetBonuses();
			auto startField = cg.GetGameBoard();

			cg.ApplyBonusClearColor(Color::RED);
			auto nextBonuses = cg.GetBonuses();
			auto nextField = cg.GetGameBoard();
			REQUIRE_FALSE(startBonuses == nextBonuses);
			REQUIRE_FALSE(startField == nextField);

			cg.CancelTurn();
			auto cancelBonuses = cg.GetBonuses();
			auto cancelField = cg.GetGameBoard();
			CHECK(startBonuses == cancelBonuses);
			CHECK(startField == cancelField);
		}
		SECTION("History work with RemoveCell bonus"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::REMOVE_CELL, 1}});

			REQUIRE_FALSE(cg.HasCancelHistory());
			auto startBonuses = cg.GetBonuses();
			auto startField = cg.GetGameBoard();

			cg.ApplyBonusRemoveCell(0, 0);
			auto nextBonuses = cg.GetBonuses();
			auto nextField = cg.GetGameBoard();
			REQUIRE_FALSE(startBonuses == nextBonuses);
			REQUIRE_FALSE(startField == nextField);

			cg.CancelTurn();
			auto cancelBonuses = cg.GetBonuses();
			auto cancelField = cg.GetGameBoard();
			CHECK(startBonuses == cancelBonuses);
			CHECK(startField == cancelField);
		}
		SECTION("History work with CangeColor bonus"){
			CollapseGame cg (2, 2, {Color::RED}, 1, {{Bonuses::CHANGE_CELL_COLOR, 1}});

			REQUIRE_FALSE(cg.HasCancelHistory());
			auto startBonuses = cg.GetBonuses();
			auto startField = cg.GetGameBoard();

			cg.ApplyBonusChangeColor(0, 0, Color::SKY);
			auto nextBonuses = cg.GetBonuses();
			auto nextField = cg.GetGameBoard();
			REQUIRE_FALSE(startBonuses == nextBonuses);
			REQUIRE_FALSE(startField == nextField);

			cg.CancelTurn();
			auto cancelBonuses = cg.GetBonuses();
			auto cancelField = cg.GetGameBoard();
			CHECK(startBonuses == cancelBonuses);
			CHECK(startField == cancelField);
		}
	}
}
