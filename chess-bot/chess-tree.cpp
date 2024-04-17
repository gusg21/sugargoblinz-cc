//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree.h"
#include "chess-tree-node.h"

namespace chess {
    ChessTreeNode* ChessTree::newNode(ChessTreeNode *parent, const chess::Move &move) {
        // We need to make a new node in the array and then return a pointer to it
        // But the object itself should be managed by this class
        ChessTreeNode* node = &nodes[nextNodeIndex++];

        // Set the proper parent
        node->setValid(true);
        node->setParent(parent);
        node->setBoard(parent->getBoard());

        // Do the move
        node->applyMove(move);

        if (parent == nullptr) {
            root = node;
        }

        // Return a reference to the pointer?
        return node;
    }

    ChessTreeNode* ChessTree::createRoot(Board board)
    {
        ChessTreeNode* node = &nodes[nextNodeIndex++];

        node->setValid(true);
        node->setParent(nullptr);
        node->setBoard(board);
        root = node;
    }

    [[nodiscard]] ChessTreeNode* ChessTree::selectNode(ChessTreeNode* node, float uctToBeat)
    {
        float bestUCT = uctToBeat;
        ChessTreeNode* bestChild = nullptr;
        for (int i = 0; i < node->getChildCount(); i++)
        {
            ChessTreeNode* child = node->getChild(i);
            float childUct = child->calculateUCT();
            if (childUct > bestUCT)
            {
                bestUCT = child->calculateUCT();
                bestChild = child;
            }
        }

        // Evaluate best child
        if (bestChild == nullptr) // Can't beat this node
        {
            return node;
        }
        else
        {
            return selectNode(bestChild, bestUCT);
        }
    }

    ChessTreeNode* ChessTree::expandNode(ChessTreeNode* node)
    {
        // calculate random move
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, node->getBoard());

        if (moves.empty())
        {
            std::cout << "bruh" << std::endl;
        }

        int randomNum = std::rand() % moves.size();

        ChessTreeNode* newChild = newNode(node, moves[randomNum]);

        return newChild; // yooooo co-op coding
    }

    float ChessTree::mcEvalNode(const ChessTreeNode* node) {
        chess::Board board = node->getBoard();

        // Play until end of game
        chess::GameResult result = board.isGameOver().second;
        while (result == GameResult::NONE) {
            chess::Movelist moves;
            chess::movegen::legalmoves(moves, board);
            board.makeMove(moves[rand() % moves.size()]);
            result = board.isGameOver().second;
        }

        // Translate into eval
        switch (result) {
            case GameResult::WIN:
                return 1;
            case GameResult::LOSE:
                return -1;
            case GameResult::DRAW:
            default:
                return 0;
        }
    }

    void ChessTree::backpropagation(ChessTreeNode* node, bool wasWin) const
    {
        ChessTreeNode* head = node;
        while (!isRoot(head))
        {
            head->incrementVisits();
            if (wasWin) head->incrementWins();
            head = head->getParent();
        }
    }
}