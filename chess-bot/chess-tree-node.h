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
        std::string moveUCI;

        std::vector<ChessTreeNode *> children;
        ChessTreeNode *parent = nullptr;

        bool valid = false;
        float value;
        int visits;

    public:
        ChessTreeNode() : board(), children(), parent(nullptr), value(0), visits(0), valid(false) {}

        /******** GETTERS *********/
        /**
         * Get the board for this node
         * @return the chess board
         */
        [[nodiscard]] chess::Board getBoard() const { return board; }

        void setValid(bool _valid) { valid = _valid; }

        [[nodiscard]] bool isValid() const { return valid; }

        /**
         * Get the value for this node
         * @return the value
         */
        [[nodiscard]] float getValue() const { return value; }

        /**
         * Get the number of times this node has been visited
         * @return the number of times visited
         */
        [[nodiscard]] int getVisits() const { return visits; }

        /**
         * Get the parent node
         * @return a pointer to the parent node
         */
        [[nodiscard]] ChessTreeNode *getParent() const { return parent; }

        /**
         * Get the number of children
         * @return the child count
         */
        [[nodiscard]] uint32_t getChildCount() const { return children.size(); }

        /**
         * Get a child at an index
         * @param index the index
         * @return the child
         */
        [[nodiscard]] ChessTreeNode* getChild(uint32_t index) const { return children[index]; }

        [[nodiscard]] std::string getMoveUCI() const { return moveUCI; }


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

        void addValue(float _value) { this->value += _value; }

        /******** METHODS *********/
        /**
         * Calculate the UCT for a given state.
         * @return the uct
         */
        [[nodiscard]] float calculateUCT() const {
            float ourVisits = static_cast<float>(getVisits());
            float valueRatio = static_cast<float>(getValue()) / ourVisits;
            float parentVisits = 0;
            if (getParent() != nullptr)
                parentVisits = static_cast<float>(getParent()->getVisits());
            float krabbyPattyFormula = sqrtf(2.f);
            return valueRatio + krabbyPattyFormula * (logf(parentVisits) / ourVisits);
        }

        /**
         * Add one to the visit counter
         */
        void incrementVisits() { visits++; }

        /**
         * Apply a moveUCI to this node's board state
         * @param moveUCI the moveUCI to apply
         */
        void applyMove(const chess::Move &moveUCI);

        void addChild(ChessTreeNode *pNode);

        void debugPrint(int depth = 0);
    };
}

#endif //CHESS_CHESS_TREE_NODE_H
