//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree-node.h"

namespace chess {
    void ChessTreeNode::applyMove(const chess::Move &move) {
        // Apply the move
        board.makeMove(move);
        moveUCI = chess::uci::moveToUci(move);
    }

    void ChessTreeNode::addChild(ChessTreeNode *node) {
        children.push_back(node);
    }

    void ChessTreeNode::debugPrint(int depth) {
#ifdef INCLUDE_PRINTS
        printf("%sNode (uct %f) (move %s) (visits %d) (value %f)\n", std::string(depth, ' ').c_str(), calculateUCT(), moveUCI.c_str(), visits, value);
#endif
        for (auto child : children) {
            if (depth < 1) child->debugPrint(depth + 1);
        }
    }
}