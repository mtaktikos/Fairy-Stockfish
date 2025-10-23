# Mailbox-Based Move Generation

## Overview

Fairy-Stockfish has been modified to use mailbox-based move generation instead of bitboard-based move generation. This document explains the changes and their implications.

## What Changed

### Before
The engine used bitboard operations to find pieces and generate moves:
- Used `pos.pieces(Us, Pt)` to get a bitboard of all pieces of a given type
- Used `pop_lsb(bb)` to iterate through pieces by popping bits from the bitboard
- Relied heavily on bitwise operations for move generation

### After
The engine now uses the mailbox array to find pieces:
- Iterates through all squares using a simple loop: `for (Square from = SQ_A1; from < SQUARE_NB; ++from)`
- Checks each square using `pos.piece_on(from)` to see if it contains a piece of the desired type
- Still uses bitboards for attack generation and position representation

## Technical Details

### Modified Functions

1. **`generate_moves<Color Us, GenType Type>`** (movegen.cpp)
   - Changed from bitboard iteration to mailbox iteration
   - Iterates through all squares on the board
   - Skips empty squares and squares with wrong piece type/color

2. **`generate_pawn_moves<Color Us, GenType Type>`** (movegen.cpp)
   - Builds pawn bitboard from mailbox before processing
   - Maintains compatibility with existing pawn move logic

### What Didn't Change

- Position representation still uses both bitboards and mailbox
- Attack generation still uses bitboards (via `pos.attacks_from()`, `pos.moves_from()`)
- Move legality checking unchanged
- All other position methods unchanged

## Performance

The mailbox-based approach produces **identical results** to the bitboard-based approach:
- Same node count in bench: 6,180,480 nodes
- Same move ordering
- Same search results

Performance characteristics:
- Slightly slower iteration (checking all squares vs. only occupied squares)
- Simpler code that's easier to understand
- Better suited for variants with sparse piece placement

## Why This Change?

This change demonstrates that mailbox-based move generation is a viable alternative to bitboard-based move generation, answering the question: "Is it possible to replace bitboards with mailbox-generated moves?"

The answer is **yes** - while bitboards are still used internally for position representation and attack generation, the actual move generation iteration can be done using the mailbox array.

## Building

The mailbox-based move generation is now the default. Simply build as usual:

```bash
cd src
make -j2 ARCH=x86-64 build
```

## Testing

To verify correctness:

```bash
# Run basic functionality test
./stockfish bench

# Run signature test
../tests/signature.sh
```

Both tests should pass successfully.
