#ifndef COLLAPSE_GAME_H
#define COLLAPSE_GAME_H

#include "cubes.h"
#include <vector>
#include <optional>
#include <unordered_map>
#include <deque>

namespace collapse {

	struct Cell {
		cubes::Color color;
		cubes::Status status;
		bool isEmpty = false;
		auto operator<=>(const Cell&) const = default;
	};
	using CellPtr = Cell*;
	using ConstCellPtr = const Cell*;

	namespace detail {
		struct Coordinates {
			int x;
			int y;
		};

		class GameBoard {
		public:
			GameBoard(int width, int height, cubes::ColorSet colors);

			void SetCellSize(int size);

			// Ожидаются координаты с началом отсчета в нуле
			CellPtr GetCellByGlobalCoord(int globalX, int globalY);

			ConstCellPtr GetCellByLocalCoord(const Coordinates & coord) const;
			CellPtr GetCellByLocalCoord(const Coordinates & coord);

			ConstCellPtr GetTop(ConstCellPtr cell) const;
			ConstCellPtr GetBottom(ConstCellPtr cell) const;
			ConstCellPtr GetLeft(ConstCellPtr cell) const;
			ConstCellPtr GetRight(ConstCellPtr cell) const;

			CellPtr GetTop(CellPtr cell);
			CellPtr GetBottom(CellPtr cell);
			CellPtr GetLeft(CellPtr cell);
			CellPtr GetRight(CellPtr cell);

			std::vector<Cell>& GetField();
			const std::vector<Cell>& GetField() const;
			int GetWidth() const;
			int GetHeight() const;

		private :
			int width;
			int height;
			std::vector<Cell> field;

			int cellSize = 1; // размер ячейки

			using Index = size_t;

			Coordinates GetLocalCoordinates(int globalX, int globalY) const;
			Coordinates GetLocalCoordinates(Index index) const;
			Index GetIndex(const Coordinates & coord) const;
		};
	}

	using ColorStatistic = std::unordered_map<cubes::Color, int>;
	using BonusContainer = std::unordered_map<cubes::Bonuses, int>;

	class CollapseGame {
	public:
		CollapseGame(int width, int height, cubes::ColorSet colors, int cellSize,
			BonusContainer initBonuses);

		// Неиспользованные бонусы
		const BonusContainer & GetBonuses() const;
		// Получить количество очков по цветам
		const ColorStatistic & GetColorStatistic() const;
		// Получить количество кубиков, оставшихся в игре
		ColorStatistic GetColorsInGame() const;

		bool HasCancelHistory() const;
		void CancelTurn();

		bool HoverHint(); // Показывает самое большое возможное выделение, если есть
		int SetHover(int x, int y);
		void ClearHover();
		void Collapse(int x, int y);

		void SetBonusHover(int x, int y);
		int SetBonusHover(cubes::Color color);

		// удалить все ячейки заданного цвета
		void ApplyBonusClearColor(cubes::Color color);
		// удалить ячейку
		void ApplyBonusRemoveCell(int x, int y);
		// заменить цвет ячейки
		void ApplyBonusChangeColor(int x, int y, cubes::Color color);

		struct GameStatus {
			bool isGameOver;
			bool isWin;
		};
		GameStatus GetGameStatus() const;

		const std::vector<Cell>& GetGameBoard() const;
	private:
		detail::GameBoard board;
		BonusContainer bonuses;
		ColorStatistic colorStatistic;

		struct Past {
			detail::GameBoard board;
			BonusContainer bonuses;
			ColorStatistic colorStatistic;
		};
		// дек чтобы легко удалять самые старые операции и добавлять новые
		std::deque<Past> history;

		int SetHover(CellPtr hoverCell);
		int MakeNeighbourActive(CellPtr cell);
		bool IsHoverable(ConstCellPtr checking, ConstCellPtr pattern) const;
		bool IsSameCell(ConstCellPtr checking, ConstCellPtr pattern) const;
		bool HasSameNeighbours(ConstCellPtr cell) const;
		void AddHistory();
		void RemoveActive();
		void MoveDown();
		void MoveLeft();
		void CompactCells();
		int ComputeScore(int amount) const;
	};
}

#endif // COLLAPSE_GAME_H
