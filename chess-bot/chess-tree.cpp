//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree.h"
#include "chess-tree-node.h"

namespace chess {
ChessTreeNode* ChessTree::newNode(ChessTreeNode* parent, const chess::Move& move)
{
    // Find a node if it already exists
    std::string moveUCI = chess::uci::moveToUci(move);
    for (uint32_t i = 0; i < parent->getChildCount(); i++) {
        ChessTreeNode* node = parent->getChild(i);
        if (node->getMoveUCI() == moveUCI) {
            return node;
        }
    }

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
    } else {
        parent->addChild(node);
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

    return node;
}

[[nodiscard]] ChessTreeNode* ChessTree::selectNode(ChessTreeNode* node)
{
    ChessTreeNode* head = getRoot();
    if (node == nullptr)
        return node;

    float bestUCT = node->calculateUCT();
    ChessTreeNode* bestNode = node;
    for (uint32_t i = 0; i < MAX_TREE_NODES; i++) {
        if (nodes[i].isValid()) {
            float uct = nodes[i].calculateUCT();
            if (nodes[i].getBoard().sideToMove() == chess::Color::WHITE)
            {
                if (uct > bestUCT) {
                    bestUCT = uct;
                    bestNode = &nodes[i];
                }
            }
            else {
                if (uct < bestUCT) {
                    bestUCT = uct;
                    bestNode = &nodes[i];
                }
            }
        }
        else {
            break;
        }
    }
    /*float uctToBeat = node->calculateUCT();
    ChessTreeNode* bestNode = nullptr;
    for (size_t i = 0; i < head->getChildCount(); i++) {
        float uct = head->getChild(i)->calculateUCT();
        if (uct > uctToBeat + 0.1f) {
            bestNode = head->getChild(i);
            uctToBeat = uct;
        }
    }*/

    if (bestNode != nullptr)
        return bestNode;
    else
        return node;
}

bool ChessTree::hasMoves(ChessTreeNode* node)
{
    // calculate random move
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, node->getBoard());

    return moves.size() > 0;
}

ChessTreeNode* ChessTree::expandNode(ChessTreeNode* node)
{
    // calculate random move
    chess::Movelist moves;
    chess::movegen::legalmoves(moves, node->getBoard());

    chess::Move chosenMove = moves[rand() % moves.size()];
    std::string _ = chess::uci::moveToUci(chosenMove);

    return newNode(node, chosenMove);
}

float ChessTree::getHeuristicValue(const chess::Board& board)
{
    chess::Color usColor = chess::Color::WHITE;
    chess::Color themColor = chess::Color::BLACK;

    float ourMaterial = ((float)board.pieces(PieceType::PAWN, usColor).count() * 5.f) + 
        ((float)board.pieces(PieceType::KNIGHT, usColor).count() * 6.f) + 
        ((float)board.pieces(PieceType::BISHOP, usColor).count() * 7.f) + 
        ((float)board.pieces(PieceType::ROOK, usColor).count() * 8.f) + 
        ((float)board.pieces(PieceType::QUEEN, usColor).count() * 9.f);
    float theirMaterial = ((float)board.pieces(PieceType::PAWN, themColor).count() * 5.f) + 
        ((float)board.pieces(PieceType::KNIGHT, themColor).count() * 6.f) + 
        ((float)board.pieces(PieceType::BISHOP, themColor).count() * 7.f) + 
        ((float)board.pieces(PieceType::ROOK, themColor).count() * 8.f) + 
        ((float)board.pieces(PieceType::QUEEN, themColor).count() * 9.f);

    return ourMaterial / 91.f - theirMaterial / 91.f;
}

float ChessTree::getMoveHeuristicValue(const chess::Board& board, const chess::Move& move) {
    float sideVal = board.sideToMove() == chess::Color::WHITE ? 1.f : -1.f;
    return (board.isCapture(move) ? 1.f : 0.f) * sideVal;
}

float ChessTree::mcEvalNode(const ChessTreeNode* node, std::string& outFen)
{
    chess::Board board = node->getBoard();

    // Play until end of game
    chess::GameResultReason reason = board.isGameOver().first;
    chess::GameResult result = board.isGameOver().second;
    uint32_t depth = 25;
    std::string _;
    while (result == GameResult::NONE && depth > 0) {
        chess::Movelist moves;
        chess::movegen::legalmoves(moves, board);

        chess::Move chosenMove {};
        float bestEval = -INFINITY;
        for (int i = 0; i < moves.size(); i++) {
            
        }
        _ = chess::uci::moveToUci(chosenMove);

        board.makeMove(chosenMove);
        _ = board.getFen();

        reason = board.isGameOver().first;
        result = board.isGameOver().second;

        depth--;
    }

    outFen = board.getFen();

    // Translate into eval
    switch (result) {
    case GameResult::WIN:
        return -1;
    case GameResult::LOSE:
        return 1;
    case GameResult::DRAW:
        // printf("%d", reason);
        return 0;
    case GameResult::NONE:
    default:
        float value = getHeuristicValue(board);
        return value;
    }
}

void ChessTree::backpropagation(ChessTreeNode* node, float eval, chess::Color rootColor) const
{
    ChessTreeNode* head = node;
    while (!isRoot(head)) {
        head->incrementVisits();
        //float flip = head->getBoard().sideToMove() == rootColor ? -1.f : 1.f;
        head->addValue(eval);
        head = head->getParent();
    }
    head->incrementVisits(); // Increment root as well
}

std::string ChessTree::getBestMove() const
{
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
#ifdef INCLUDE_PRINTS
    root->debugPrint(0);
#endif
}
}