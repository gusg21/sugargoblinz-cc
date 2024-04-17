//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree.h"
#include "chess-tree-node.h"

namespace chess {
    ChessTreeNode *ChessTree::newNode(ChessTreeNode *parent, const chess::Move &move) {
        // We need to make a new node in the array and then return a pointer to it
        // But the object itself should be managed by this class
        ChessTreeNode *node = &nodes[nextNodeIndex++];

        // Set the proper parent
        node->setValid(true);
        node->setParent(parent);
        node->setBoard(parent->getBoard());

        // Do the move
        node->applyMove(move);

        if (parent == nullptr) {
            root = node;
        } else {
            parent->addChild(node);
        }

        // Return a reference to the pointer?
        return node;
    }

    ChessTreeNode *ChessTree::createRoot(Board board) {
        ChessTreeNode *node = &nodes[nextNodeIndex++];

        node->setValid(true);
        node->setParent(nullptr);
        node->setBoard(board);
        root = node;

        return node;
    }

    [[nodiscard]] ChessTreeNode *ChessTree::selectNode(ChessTreeNode *node, float uctToBeat) {
        float bestUCT = uctToBeat;
        ChessTreeNode *bestChild = nullptr;
        for (int i = 0; i < node->getChildCount(); i++) {
            ChessTreeNode *child = node->getChild(i);

            if (!hasMoves(child)) {
                continue;
            }

            float childUct = child->calculateUCT();
            if (childUct > bestUCT) {
                bestUCT = child->calculateUCT();
                bestChild = child;
            }
        }

        // Evaluate best child
        if (bestChild == nullptr) // Can't beat this node
        {
            return node;
        } else {
            return selectNode(bestChild, bestUCT);
        }
    }

    bool ChessTree::hasMoves(ChessTreeNode *node) {
        // calculate random move
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, node->getBoard());

        return moves.size() > 0;
    }

    ChessTreeNode *ChessTree::expandNode(ChessTreeNode *node) {
        // calculate random move
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, node->getBoard());

        if (moves.empty()) {
            printf("ERROR: Bruh\n");
        }

        int randomNum = std::rand() % moves.size();

        ChessTreeNode *newChild = newNode(node, moves[randomNum]);

        return newChild; // yooooo co-op coding
    }

    float ChessTree::getHeuristicValue(const chess::Board &board) const {
        chess::Color usColor = board.sideToMove();
        chess::Color themColor = ~board.sideToMove();

        float ourMaterial =
                (board.pieces(PieceType::PAWN, usColor).count() * 1.f) +
                (board.pieces(PieceType::KNIGHT, usColor).count() * 3.f) +
                (board.pieces(PieceType::BISHOP, usColor).count() * 3.f) +
                (board.pieces(PieceType::ROOK, usColor).count() * 5.f) +
                (board.pieces(PieceType::QUEEN, usColor).count() * 9.f);
        float theirMaterial =
                (board.pieces(PieceType::PAWN, themColor).count() * 1.f) +
                (board.pieces(PieceType::KNIGHT, themColor).count() * 3.f) +
                (board.pieces(PieceType::BISHOP, themColor).count() * 3.f) +
                (board.pieces(PieceType::ROOK, themColor).count() * 5.f) +
                (board.pieces(PieceType::QUEEN, themColor).count() * 9.f);

        return (ourMaterial / 50.f) - (theirMaterial / 50.f);
    }

    float ChessTree::mcEvalNode(const ChessTreeNode *node) {
        chess::Board board = node->getBoard();

        // Play until end of game
        chess::GameResult result = board.isGameOver().second;
        uint32_t depth = 50;
        while (result == GameResult::NONE && depth > 0) {
            chess::Movelist moves;
            chess::movegen::legalmoves(moves, board);

            chess::Move bestMove{};
            float bestVal = -1.0f;
            for (uint32_t i = 0; i < moves.size(); i++) {
                chess::Board newBoard = board;
                newBoard.makeMove(moves[(int) i]);
                float thisVal = getHeuristicValue(newBoard);
                if (thisVal > bestVal) {
                    bestVal = thisVal;
                    bestMove = moves[(int) i];

                    if (thisVal == 1.0f) {
                        break;
                    }
                }
            }

            board.makeMove(bestMove);
            result = board.isGameOver().second;
            depth--;
        }

        // Translate into eval
        switch (result) {
            case GameResult::WIN:
                return 1;
            case GameResult::LOSE:
                return -1;
            case GameResult::DRAW:
                return 0;
            default:
                return getHeuristicValue(board);
        }
    }

    void ChessTree::backpropagation(ChessTreeNode *node, float eval) const {
        ChessTreeNode *head = node;
        while (!isRoot(head)) {
            head->incrementVisits();
            head->addValue(eval);
            head = head->getParent();
        }
        head->incrementVisits(); // Increment root as well
        head->addValue(eval);
    }

    std::string ChessTree::getBestMove() const {
        int bestIndex = -1;
        float bestUCT = -100000;
        for (int childIndex = 0; childIndex < root->getChildCount(); childIndex++) {
            float currentUCT = root->getChild(childIndex)->calculateUCT();
            if (currentUCT > bestUCT) {
                bestUCT = currentUCT;
                bestIndex = childIndex;
            }
        }

        return root->getChild(bestIndex)->getMoveUCI();
    }

    void ChessTree::debugPrint() const
    {
        root->debugPrint(0);
    }
}