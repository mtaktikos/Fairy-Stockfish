# Transparent Squares Feature

## Overview

The transparent squares feature allows certain piece types to create "transparent" regions around them that friendly slider pieces (Queen, Rook, Bishop) can slide through.

## How It Works

1. **Transparent Squares**: Squares adjacent to or containing a piece of a transparent piece type are called "transparent squares" for that color.
   - For white: squares adjacent to or containing white transparent pieces are "whitetransparent"
   - For black: squares adjacent to or containing black transparent pieces are "blacktransparent"

2. **Slider Movement**: 
   - White sliders (Q, R, B, but NOT the transparent piece itself) can slide THROUGH whitetransparent squares
   - Black sliders (Q, R, B, but NOT the transparent piece itself) can slide THROUGH blacktransparent squares
   - Sliders can only LAND on:
     - Empty squares
     - Squares containing enemy pieces (capturing them)

## Configuration

To enable this feature in a variant, use the `transparentPieceTypes` configuration option:

```ini
[my_variant:chess]
transparentPieceTypes = n  # Makes knights create transparent squares
```

## Example

In the included test variant (`tests/transparent_test.ini`), knights are designated as transparent pieces:

```ini
[transparent_test:chess]
transparentPieceTypes = n
```

### Effect on Gameplay

With a white knight on e4:
- Squares d3, e3, f3, d4, e4, f4, d5, e5, f5 become whitetransparent
- A white rook on e1 can now slide through e2, e3 to reach e4+ (if e4+ is empty or has a black piece)
- In regular chess, the rook would be blocked by the knight on e4's influence

### Testing

Run the comparison test to see the feature in action:

```bash
cd tests
./test_transparent_comparison.sh
```

This compares move counts between regular chess and the transparent variant, showing that sliders have more mobility with transparent squares enabled.

## Implementation Details

The feature is implemented by:
1. Calculating transparent squares in `Position::set_check_info()` based on piece positions
2. Modifying `Position::attacks_from()` and `Position::moves_from()` to exclude transparent squares from the occupied bitboard for slider pieces
3. Move generation already filters moves to ensure pieces can only land on valid squares (empty or enemy-occupied)

## Use Cases

This feature is useful for creating chess variants where certain pieces (like a "Witch" or similar piece) have the ability to allow friendly pieces to move more freely by making nearby squares "passable" for sliders.
