//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree.h"
#include "chess-tree-node.h"

namespace chess {
    ChessTreeNode* ChessTree::newNode(const ChessTreeNode *parent, const chess::Move &move) {
        // We need to make a new node in the array and then return a pointer to it
        // But the object itself should be managed by this class
        nodes.emplace_back();
        ChessTreeNode* node = &nodes.back(); // Creates a new object

        // Set the proper parent
        node->setParent(parent);
        node->setBoard(parent->getBoard());

        // Do the move
        node->applyMove(move);

        // Return a reference to the pointer?
        return node;
    }

    [[nodiscard]] const ChessTreeNode* ChessTree::selectNode(const ChessTreeNode* node, float uctToBeat)
    {
        float bestUCT = uctToBeat;
        const ChessTreeNode* bestChild = nullptr;
        for (int i = 0; i < node->getChildCount(); i++)
        {
            const ChessTreeNode* child = node->getChild(i);
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

    ChessTreeNode* ChessTree::expandNode(const ChessTreeNode* node)
    {
        // calculate random move
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, node->getBoard());

        if (moves.empty())
        {
            std::cout << "bruh" << std::endl;
        }

        int randomNum = std::rand() % moves.size();

        const ChessTreeNode* newChild = newNode(node, moves[randomNum]);

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