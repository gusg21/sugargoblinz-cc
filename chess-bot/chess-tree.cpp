//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree.h"
#include "chess-tree-node.h"

namespace chess {
    ChessTreeNode &ChessTree::newNode(ChessTreeNode *parent, const chess::Move &move) {
        // We need to make a new node in the array and then return a pointer to it
        // But the object itself should be managed by this class
        ChessTreeNode &node = nodes.emplace_back(); // Creates a new object

        // Set the proper parent
        node.setParent(parent);
        node.setBoard(parent->getBoard());

        // Do the move
        node.applyMove(move);

        // Return a reference to the pointer?
        return node;
    }

    [[nodiscard]] ChessTreeNode& ChessTree::selectNode(const ChessTreeNode& node)
    {
        float bestUCT = -100000.f; // We are realists
        ChessTreeNode pointToBest;
        for (int i = 0; i < nodes.capacity(); i++)
        {
            ChessTreeNode node = nodes[i];
            float nodeUct = node.calculateUCT();
            if (nodeUct > bestUCT)
            {
                bestUCT = node.calculateUCT();
                pointToBest = node;
            }
        }
        ChessTreeNode& result = pointToBest;

        //if pointToBest has children
        if (pointToBest.GetChildrenCount())
        {
            result = selectNode(pointToBest);
        }

        return result;
    }

    ChessTreeNode &ChessTree::expandNode(const ChessTreeNode &node) {
        return nullptr;
    }

    float ChessTree::mcEvalNode(const ChessTreeNode& node) {
        chess::Board board = node.getBoard();

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

}