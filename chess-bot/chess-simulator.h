#pragma once
#include <string>
#include <memory>
#include <utility>
#include "chess.hpp"

namespace ChessSimulator {

    class ChessTreeNode {
    private:
        chess::Board board;

        std::vector<ChessTreeNode*> children;
        ChessTreeNode* parent = nullptr;

        int value; // Needed?
        int visits;

    public:
        // Getters
        [[nodiscard]] const chess::Board* getBoard() const { return &board; }
        [[nodiscard]] int getValue() const { return value; }
        [[nodiscard]] int getVisitCount() const { return visits; }
        [[nodiscard]] ChessTreeNode* getParent() const { return parent; }

        // Setters
        void setParent(ChessTreeNode* node) { this->parent = node; }
        void setBoard(chess::Board _board) { this->board = std::move(_board); }

        // Methods
        [[nodiscard]] float calculateUCT() const { return 0.f; }

    };

    class ChessTree {
    private:
        std::vector<ChessTreeNode> nodes;
        ChessTreeNode* root = nullptr;

    public:
        ChessTree();

        ChessTreeNode* newNode(ChessTreeNode* parent, const chess::Move& move);

        [[nodiscard]] bool isRoot(ChessTreeNode* node) const { return node == root; }
    };

/**
 * @brief Move a piece on the board
 *
 * @param fen The board as FEN
 * @return std::string The move as UCI
 */
std::string Move(std::string fen);

} // namespace ChessSimulator