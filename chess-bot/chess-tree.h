//
// Created by angus.goucher on 4/4/2024.
//

#ifndef CHESS_CHESS_TREE_H
#define CHESS_CHESS_TREE_H

#include "chess-tree-node.h"

namespace chess {
    class ChessTree {
    private:
        std::vector<ChessTreeNode> nodes;
        ChessTreeNode* root = nullptr;

    public:
        ChessTree() = default;

        // Methods
        /**
         * Create a new node from a parent state and a given move
         * @param parent the parent state
         * @param move the move to make
         * @return a reference to the newly create node (managed by the tree)
         */
        [[nodiscard]] ChessTreeNode& newNode(ChessTreeNode* parent, const chess::Move& move);

        /**
         * Is a given node the root node?
         * @param node the node to check
         * @return yea or nay
         */
        [[nodiscard]] bool isRoot(ChessTreeNode* node) const { return node == root; }

        /**
         * Do MCTS selection on this tree
         * @return a reference to the selected node
         */
        [[nodiscard]] ChessTreeNode& selectNode(const ChessTreeNode& node);

        /**
         * Expand a given node
         * @param node the node to expand from
         * @return a reference to the new node
         */
        [[nodiscard]] ChessTreeNode& expandNode(const ChessTreeNode& node);

        /**
         * Run a game to an end condition, return the result
         * @param node the node to run from
         * @return the outcome
         */
        [[nodiscard]] float mcEvalNode(const ChessTreeNode& node);
    };
};


#endif //CHESS_CHESS_TREE_H
