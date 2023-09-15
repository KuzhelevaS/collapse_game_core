#include "collapse_game.h"

#include <random>
#include <cassert>
#include <iostream>

namespace collapse {
	using namespace detail;

	GameBoard::GameBoard(int width, int height, cubes::ColorSet colors)
		: width(width), height(height), field(std::vector<Cell>(width * height))
	{
		static std::random_device rd; // Only used once to initialise (seed) engine
		std::mt19937 rng(rd()); // Random-number engine used (Mersenne-Twister in this case)
		std::uniform_int_distribution<int> uni(0, colors.size() - 1); // Guaranteed unbiased

		for (auto & cell : field) {
			auto random_integer = uni(rng);
			cell = {colors.at(random_integer), cubes::Status::NORMAL};
		}
	}

	void GameBoard::SetCellSize(int size) {
		this->cellSize = size;
	}

	std::vector<Cell>& GameBoard::GetField() {
		return field;
	}

	const std::vector<Cell>& GameBoard::GetField() const {
		return field;
	}

	CellPtr GameBoard::GetCellByGlobalCoord(int globalX, int globalY) {
		Coordinates localCoords = GetLocalCoordinates(globalX, globalY);
		Index id = GetIndex(localCoords);
		return &field.at(id);
	}

	ConstCellPtr GameBoard::GetTop(ConstCellPtr cell) const {
		Index id = cell - field.data();
		Coordinates coord = GetLocalCoordinates(id);
		--coord.y;
		if (coord.y < 0) {
			return nullptr;
		}
		return GetCellByLocalCoord(coord);
	}

	ConstCellPtr GameBoard::GetBottom(ConstCellPtr cell) const {
		Index id = cell - field.data();
		Coordinates coord = GetLocalCoordinates(id);
		++coord.y;
		if (coord.y >= height) {
			return nullptr;
		}
		return GetCellByLocalCoord(coord);
	}

	ConstCellPtr GameBoard::GetLeft(ConstCellPtr cell) const {
		Index id = cell - field.data();
		Coordinates coord = GetLocalCoordinates(id);
		--coord.x;
		if (coord.x < 0) {
			return nullptr;
		}
		return GetCellByLocalCoord(coord);
	}

	ConstCellPtr GameBoard::GetRight(ConstCellPtr cell) const {
		Index id = cell - field.data();
		Coordinates coord = GetLocalCoordinates(id);
		++coord.x;
		if (coord.x >= width) {
			return nullptr;
		}
		return GetCellByLocalCoord(coord);
	}

	CellPtr GameBoard::GetTop(CellPtr cell) {
		return const_cast<CellPtr>(const_cast<const GameBoard &>(*this).GetTop(cell));
	}

	CellPtr GameBoard::GetBottom(CellPtr cell) {
		return const_cast<CellPtr>(const_cast<const GameBoard &>(*this).GetBottom(cell));
	}

	CellPtr GameBoard::GetLeft(CellPtr cell) {
		return const_cast<CellPtr>(const_cast<const GameBoard &>(*this).GetLeft(cell));
	}

	CellPtr GameBoard::GetRight(CellPtr cell) {
		return const_cast<CellPtr>(const_cast<const GameBoard &>(*this).GetRight(cell));
	}


	Coordinates GameBoard::GetLocalCoordinates(int globalX, int globalY) const {
		assert (globalX >= 0 && globalX < width * cellSize);
		assert (globalY >= 0 && globalY < height * cellSize);

		int x = globalX / cellSize;
		int y = globalY / cellSize;
		return {x, y};
	}

	Coordinates GameBoard::GetLocalCoordinates(Index index) const {
		assert (index < field.size());

		int x = index % width;
		int y = index / width;
		return {x, y};
	}

	GameBoard::Index GameBoard::GetIndex(const Coordinates & coord) const {
		assert (coord.x >= 0 && coord.x < width);
		assert (coord.y >= 0 && coord.y < height);

		return coord.y * width + coord.x;
	}

	ConstCellPtr GameBoard::GetCellByLocalCoord(const Coordinates & coord) const {
		Index id = GetIndex(coord);
		return &field.at(id);
	}

	CellPtr GameBoard::GetCellByLocalCoord(const Coordinates & coord) {
		return const_cast<CellPtr>(const_cast<const GameBoard &>(*this).GetCellByLocalCoord(coord));
	}

	int GameBoard::GetWidth() const {
		return width;
	}

	int GameBoard::GetHeight() const {
		return height;
	}

	CollapseGame::CollapseGame(int width, int height, cubes::ColorSet colors,
			int cellSize, BonusContainer initBonuses)
		: board(width, height, colors), bonuses(initBonuses)
	{
		board.SetCellSize(cellSize);
	}

	const BonusContainer & CollapseGame::GetBonuses() const {
		return bonuses;
	}

	const ColorStatistic & CollapseGame::GetColorStatistic() const {
		return colorStatistic;
	}

	ColorStatistic CollapseGame::GetColorsInGame() const {
		ColorStatistic result;
		for (const auto & cell : board.GetField()) {
			if (!cell.isEmpty) {
				++result[cell.color];
			}
		}
		return result;
	}

	bool CollapseGame::HasCancelHistory() const {
		return !history.empty();
	}

	void CollapseGame::CancelTurn () {
		if (HasCancelHistory()) {
			std::swap(board, history.back().board);
			std::swap(bonuses, history.back().bonuses);
			std::swap(colorStatistic, history.back().colorStatistic);
			history.pop_back();
		}
	}

	const std::vector<Cell>& CollapseGame::GetGameBoard() const {
		return board.GetField();
	}

	bool CollapseGame::HoverHint() {
		std::pair<int, CellPtr> maxHover{0, nullptr};
		auto & cells = board.GetField();
		for (auto & cell: cells) {
			int size = SetHover(&cell);
			if (size > 0 && size > maxHover.first) {
				maxHover.first = size;
				maxHover.second = &cell;
			}
		}
		if (maxHover.second) {
			SetHover(maxHover.second);
			return true;
		} else {
			return false;
		}
	}

	void CollapseGame::ClearHover() {
		for (auto & cell : board.GetField()) {
			cell.status = cubes::Status::NORMAL;
		}
	}

	int CollapseGame::SetHover(CellPtr hoverCell) {
		ClearHover();
		int score = MakeNeighbourActive(hoverCell);
		if (score == 1) { // значит у клетки нет соседей того же цвета
			hoverCell->status = cubes::Status::NORMAL;
			return 0;
		}
		return ComputeScore(score);
	}

	int CollapseGame::SetHover(int x, int y) {
		CellPtr hoverCell = board.GetCellByGlobalCoord(x, y);
		return SetHover(hoverCell);
	}

	int CollapseGame::MakeNeighbourActive(CellPtr cell) {
		if (cell->isEmpty) {
			return 0;
		}
		int result = 1;
		cell->status = cubes::Status::ACTIVE;
		if (IsHoverable(board.GetTop(cell), cell)) {
			result += MakeNeighbourActive(board.GetTop(cell));
		}
		if (IsHoverable(board.GetLeft(cell), cell)) {
			result += MakeNeighbourActive(board.GetLeft(cell));
		}
		if (IsHoverable(board.GetBottom(cell), cell)) {
			result += MakeNeighbourActive(board.GetBottom(cell));
		}
		if (IsHoverable(board.GetRight(cell), cell)) {
			result += MakeNeighbourActive(board.GetRight(cell));
		}
		return result;
	}

	bool CollapseGame::IsHoverable(ConstCellPtr checking, ConstCellPtr pattern) const {
		return IsSameCell(checking, pattern)
			&& checking->status == cubes::Status::NORMAL; // если active, то уже были в этой ячейке
	}

	bool CollapseGame::IsSameCell(ConstCellPtr checking, ConstCellPtr pattern) const {
		return checking && !checking->isEmpty
			&& checking->color == pattern->color;
	}

	bool CollapseGame::HasSameNeighbours(ConstCellPtr cell) const {
		return IsSameCell(board.GetTop(cell), cell)
			|| IsSameCell(board.GetBottom(cell), cell)
			|| IsSameCell(board.GetRight(cell), cell)
			|| IsSameCell(board.GetLeft(cell), cell);
	}

	void CollapseGame::Collapse(int x, int y) {
		AddHistory();

		CellPtr hoverCell = board.GetCellByGlobalCoord(x, y);
		// Чтобы быть уверенными, что схлопываем элементы для выбранной позиции
		int score = SetHover(x, y);

		colorStatistic[hoverCell->color] += score;

		RemoveActive();
		ClearHover();
		CompactCells();
	}

	void CollapseGame::AddHistory() {
		constexpr static int MEMORY_SIZE = 100;
		if (history.size() >= MEMORY_SIZE) {
			history.pop_front();
		}
		history.push_back({board, bonuses, colorStatistic});
	}

	void CollapseGame::RemoveActive() {
		for (auto & cell : board.GetField()) {
			if (cell.status == cubes::Status::ACTIVE) {
				cell.isEmpty = true;
			}
		}
	}

	void CollapseGame::CompactCells() {
		MoveDown();
		MoveLeft();
	}

	void CollapseGame::MoveDown() {
		int index = 11;
		for (auto it = board.GetField().rbegin(); it != board.GetField().rend(); ++it, --index) {
			if (it->isEmpty) {
				continue;
			}
			CellPtr cell = &(*it);
			CellPtr bottom = board.GetBottom(cell);
			if (bottom && bottom->isEmpty) {
				// Ищем самую нижнюю пустую ячейку
				CellPtr next = board.GetBottom(bottom);
				while (next && next->isEmpty) {
					bottom = next;
					next = board.GetBottom(next);
				}
				//Перемещаем туда текущую
				std::swap(*cell, *bottom);
			}
		}
	}

	void CollapseGame::MoveLeft() {
		// Двигаемся по нижнему ряду для анализа столбцов
		for (int i = 0; i < board.GetWidth(); ++i) {
			CellPtr column_cell = board.GetCellByLocalCoord({i, board.GetHeight() - 1});
			if (column_cell->isEmpty) {
				// Если ячейка пустая, ищем ближайший столбец с непустой нижней ячейкой
				for (int j = i + 1; j < board.GetWidth(); ++j) {
					CellPtr swaping_cell = board.GetCellByLocalCoord({j, board.GetHeight() - 1});
					if (!swaping_cell->isEmpty) {
						// Если столбец не пустой, меняем его местами с i-м столбцом
						for (int h = 0; h < board.GetHeight(); ++h) {
							CellPtr right = board.GetCellByLocalCoord({j, h});
							CellPtr left = board.GetCellByLocalCoord({i, h});
							std::swap(*left, *right);
						}
						break;
					}
				}
			}
		}
	}

	CollapseGame::GameStatus CollapseGame::GetGameStatus() const {
		bool hasNeighbour = false;
		bool isEmptyField = true;
		for (auto & cell : board.GetField()) {
			if (!cell.isEmpty) {
				isEmptyField = false;
				if (HasSameNeighbours(&cell)) {
					hasNeighbour = true;
				}
			}
		}
		return {!hasNeighbour, isEmptyField};
	}

	void CollapseGame::SetBonusHover(int x, int y) {
		ClearHover();
		CellPtr cell = board.GetCellByGlobalCoord(x, y);
		cell->status = cubes::Status::ACTIVE;
	}

	int CollapseGame::SetBonusHover(cubes::Color color) {
		ClearHover();
		int count = 0;
		for (auto & cell : board.GetField()) {
			if (cell.color == color) {
				cell.status = cubes::Status::ACTIVE;
				++count;
			}
		}
		return ComputeScore(count);
	}

	int CollapseGame::ComputeScore(int amount) const {
		return amount * amount;
	}

	void CollapseGame::ApplyBonusClearColor(cubes::Color color) {
		if (bonuses[cubes::Bonuses::CLEAR_SAME_COLOR] > 0) {
			AddHistory();
			int count = 0;
			for (auto & cell : board.GetField()) {
				if (cell.color == color) {
					cell.isEmpty = true;
					++count;
				}
			}
			colorStatistic[color] += ComputeScore(count);
			CompactCells();
			--bonuses[cubes::Bonuses::CLEAR_SAME_COLOR];
		}
	}

	void CollapseGame::ApplyBonusRemoveCell(int x, int y) {
		if (bonuses[cubes::Bonuses::REMOVE_CELL] > 0) {
			AddHistory();
			CellPtr cell = board.GetCellByGlobalCoord(x, y);
			cell->isEmpty = true;
			CompactCells();
			--bonuses[cubes::Bonuses::REMOVE_CELL];
		}
	}

	void CollapseGame::ApplyBonusChangeColor(int x, int y, cubes::Color color) {
		if (bonuses[cubes::Bonuses::CHANGE_CELL_COLOR] > 0) {
			AddHistory();
			CellPtr cell = board.GetCellByGlobalCoord(x, y);
			cell->color = color;
			--bonuses[cubes::Bonuses::CHANGE_CELL_COLOR];
		}
	}

}

