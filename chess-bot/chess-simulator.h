#pragma once
#include <string>
#include "chess.hpp"

namespace ChessSimulator {
/**
 * @brief Move a piece on the board
 *
 * @param fen The board as FEN
 * @return std::string The move as UCI
 */
std::string Move(std::string fen);

float MeasureStateJuiciness(std::string fen, chess::Color side);

} // namespace ChessSimulator