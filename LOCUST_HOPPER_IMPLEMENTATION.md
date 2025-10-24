# Locust Hopper Implementation

## Overview

This implementation adds support for "locust hopper" pieces, also known as diagonal cannons that capture the hurdle instead of the landing square.

## Behavior

Locust hoppers have the following characteristics:
1. **Hop over a piece** (the "hurdle") like regular hoppers/cannons
2. **Land ONLY on EMPTY squares** beyond the hurdle
3. **Capture the hurdle** (the piece jumped over), not the piece at the landing square

This is different from standard hoppers which:
- Can land on occupied or empty squares
- Capture the piece at the landing square (if occupied)

## Betza Notation

Use the `o` modifier before the hopper prefix to indicate locust behavior:
- `opB` = locust hopper-bishop (diagonal cannon, unlimited range)
- `opB2` = locust hopper-bishop with distance limit 2
- `opR` = locust hopper-rook (orthogonal cannon)

## Example Usage

```ini
[diagonal_cannon:chess]
pieceToCharTable = PNBRQ.C.........Q..K.pnbrq.c.........q..k
# Q = diagonal cannon (locust hopper-bishop)
customPiece2 = q:opB
# C = limited diagonal cannon
customPiece1 = c:opB2
```

## Test Case

Position: Q on a1, black pawn on c3
- Q can hop over c3 and land on d4, e5, f6, g7, or h8 (all empty squares)
- The pawn on c3 is captured (the hurdle)
- Q cannot land on c3 or any occupied square after c3

## Implementation Details

### Files Modified
1. **src/piece.h** - Added `locust` flag to PieceInfo
2. **src/piece.cpp** - Added parsing for `o` modifier in Betza notation
3. **src/movegen.cpp** - Special handling for locust pieces in move generation
4. **src/position.cpp** - Hurdle capture calculation during move execution
5. **src/position.h** - Removed from version as changes were reverted
6. **src/bitboard.cpp** - Added LOCUST to MovementType enum

### Technical Approach
- Locust hopper moves are marked as EN_PASSANT type
- During move generation, only empty squares are included for locust pieces
- During move execution, the hurdle square is calculated using `between_bb()`
- The hurdle is captured instead of the destination square

## Testing

Run with the test variant:
```bash
./stockfish
setoption name VariantPath value /tmp/final_test_locust.ini
setoption name UCI_Variant value locust_test
position fen 8/8/8/8/8/2p5/8/Q7 w - - 0 1
go perft 1
```

Expected output: Q can move to d4, e5, f6, g7, h8 (5 moves)
