//
// Created by angus.goucher on 4/4/2024.
//

#ifndef CHESS_CHESS_TREE_NODE_H
#define CHESS_CHESS_TREE_NODE_H

#include <utility>
#include <memory>
#include <string>
#include <random>
#include "chess.hpp"

namespace chess {
    class ChessTreeNode {
    private:
        chess::Board board;

        std::vector<ChessTreeNode *> children;
        ChessTreeNode *parent = nullptr;

        int value; // Needed?
        int visits;

    public:
        /******** GETTERS *********/
        /**
         * Get the board for this node
         * @return the chess board
         */
        [[nodiscard]] chess::Board getBoard() const { return board; }

        /**
         * Get the value for this node
         * @return the value
         */
        [[nodiscard]] int getValue() const { return value; }

        /**
         * Get the number of times this node has been visited
         * @return the number of times visited
         */
        [[nodiscard]] int getVisitCount() const { return visits; }

        /**
         * Get the parent node
         * @return a pointer to the parent node
         */
        [[nodiscard]] ChessTreeNode *getParent() const { return parent; }

        /**
         * Get the number of children in this node
         * @return the value
         */
        [[nodiscard]] int GetChildrenCount() { return children.capacity(); }

        /******** SETTERS *********/
        /**
         * Set the parent node
         * @param node a pointer to the parent node
         */
        void setParent(ChessTreeNode *node) { this->parent = node; }

        /**
         * Set the board for this node
         * @param _board the board to apply
         */
        void setBoard(chess::Board _board) { this->board = std::move(_board); }

        /******** METHODS *********/
        /**
         * Calculate the UCT for a given state.
         * @return the uct
         */
        [[nodiscard]] float calculateUCT() const { return 0.f; }

        /**
         * Apply a move to this node's board state
         * @param move the move to apply
         */
        void applyMove(const chess::Move &move);

    };
}

#endif //CHESS_CHESS_TREE_NODE_H
