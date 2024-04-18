#pragma once

#include <string>
#include <memory>
#include <utility>
#include "chess.hpp"
#include "chess-tree-node.h"
#include "chess-tree.h"

namespace ChessSimulator {
    /**
     * @brief Move a piece on the board
     *
     * @param fen The board as FEN
     * @return std::string The move as UCI
     */
    std::string Move(const std::string& fen);

} // namespace ChessSimulator