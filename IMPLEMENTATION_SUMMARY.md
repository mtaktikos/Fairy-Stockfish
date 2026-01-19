# Transparent Squares Implementation Summary

## Overview
This implementation adds support for transparent squares in Fairy-Stockfish, allowing piece types to create "transparent" regions that friendly slider pieces can pass through.

## What Was Implemented

### 1. Core Data Structures
- **StateInfo**: Added `transparentSquares[COLOR_NB]` to track transparent squares for each color
- **Variant**: Added `transparentPieceTypes` PieceSet to specify which piece types create transparent squares

### 2. Transparent Square Calculation
Modified `Position::set_check_info()` to calculate transparent squares:
- For each piece of a transparent piece type
- All adjacent squares (8 surrounding squares) plus the square containing the piece become transparent
- Calculated separately for white and black pieces

### 3. Slider Movement Logic
Modified `Position::attacks_from()` and `Position::moves_from()`:
- For slider pieces (those with AttackRiderTypes/MoveRiderTypes set)
- Excludes transparent squares from the occupied bitboard
- This allows sliders to "see through" transparent squares
- The transparent piece type itself does NOT get this ability

### 4. Configuration Support
- Added parser support in `parser.cpp` for `transparentPieceTypes` configuration option
- Updated `variants.ini` with documentation
- Format: `transparentPieceTypes = n` (for knights, for example)

## How It Works

### Rule Implementation
As specified in the requirements:
1. ✅ Squares adjacent to a white transparent piece or containing a white transparent piece are "whitetransparent"
2. ✅ Squares adjacent to a black transparent piece or containing a black transparent piece are "blacktransparent"
3. ✅ Transparent square flags change with every turn (recalculated in set_check_info)
4. ✅ White sliders (Q, R, B, but NOT the transparent piece itself) can slide THROUGH whitetransparent squares
5. ✅ Black sliders (Q, R, B, but NOT the transparent piece itself) can slide THROUGH blacktransparent squares
6. ✅ Sliders can only LAND on empty squares or squares with enemy pieces

### Technical Details
The implementation works by:
1. Computing transparent squares based on piece positions (in `set_check_info`)
2. Modifying the occupied bitboard passed to attack generation for sliders
3. Existing move generation logic already handles proper landing rules (can't land on friendly pieces)

## Testing

### Test Variant
Created `tests/transparent_test.ini` with a custom Witch piece (symbol 'w') as the transparent piece

### Test Results
With a white Witch (W) on e4 and white rook on e1:
- **Regular chess**: 19 moves total (with a regular knight)
- **With transparent squares**: 23 moves total (+4 moves)
- The rook can now reach e5-e8 by sliding through the transparent squares around the Witch

With a black witch (w) on e4 and black rook on e8:
- **With transparent squares**: 23 moves total
- The black rook can slide through blacktransparent squares created by the black witch

### Verified Behavior
1. ✅ Rook can slide through transparent squares vertically
2. ✅ Bishop can slide through transparent squares diagonally  
3. ✅ Queen can slide through transparent squares (both directions)
4. ✅ Sliders cannot land on friendly pieces even on transparent squares
5. ✅ Sliders CAN capture enemy pieces on transparent squares
6. ✅ No regression in standard chess (perft matches expected values)

## Files Modified

### Core Implementation
- `src/position.h`: Added accessor methods and modified movement functions
- `src/position.cpp`: Added transparent square calculation
- `src/variant.h`: Added transparentPieceTypes field
- `src/parser.cpp`: Added parsing support

### Documentation & Tests
- `src/variants.ini`: Added configuration documentation
- `TRANSPARENT_SQUARES.md`: Comprehensive feature documentation
- `tests/transparent_test.ini`: Test variant configuration
- `tests/test_transparent_comparison.sh`: Comparison test script
- `tests/test_transparent_comprehensive.sh`: Comprehensive test script

## Usage Example

To create a variant with transparent squares:

```ini
[my_witch_variant:chess]
# Define a custom Witch piece with symbol 'w'
customPiece1 = w:N  # Witch moves like a knight
# Make the Witch create transparent squares
transparentPieceTypes = w
pieceToCharTable = PNBRQ..............WKpnbrq..............wk
```

You can also use any existing piece type:
```ini
[transparent_knights:chess]
# Make knights create transparent squares
transparentPieceTypes = n
```

## Performance Impact
- Minimal: One additional bitboard calculation per move in `set_check_info`
- No impact on variants that don't use the feature (transparentPieceTypes defaults to NO_PIECE_SET)
- No change to existing chess or variant gameplay

## Compatibility
- ✅ Backward compatible: No changes to existing variants
- ✅ All existing tests pass
- ✅ Standard chess perft values unchanged
- ✅ Build succeeds without warnings
