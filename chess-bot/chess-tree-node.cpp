//
// Created by angus.goucher on 4/4/2024.
//

#include "chess-tree-node.h"

namespace chess {
    void ChessTreeNode::applyMove(const chess::Move &move) {
        // Apply the move
        board.makeMove(move);
    }
}