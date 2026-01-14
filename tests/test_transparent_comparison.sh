#!/bin/bash
# Compare transparent vs non-transparent variants to verify the feature works

cd "$(dirname "$0")/../src"

echo "=== Comparing Transparent Squares Feature ==="
echo ""

# Test with transparent variant (knight creates transparent squares)
echo "Position: White rook on e1, white knight on e4"
echo "In transparent variant, rook should be able to move through knight's adjacent squares"
echo ""

# Get moves with transparent feature enabled
transparent_result=$(echo -e "load ../tests/transparent_test.ini\nsetoption name UCI_Variant value transparent_test\nposition fen 4k3/8/8/8/4N3/8/8/4R2K w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep "Nodes searched")
echo "WITH transparent squares: $transparent_result"

# Get moves with regular chess (no transparent)
regular_result=$(echo -e "setoption name UCI_Variant value chess\nposition fen 4k3/8/8/8/4N3/8/8/4R2K w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep "Nodes searched")
echo "WITHOUT transparent squares (regular chess): $regular_result"

echo ""
echo "The transparent variant should have MORE moves for the rook because it can slide through"
echo "the transparent squares created by the knight."
echo ""

# Test 2: More complex position
echo "Position 2: White queen on e1, white knight on e4"
echo ""

transparent_result2=$(echo -e "load ../tests/transparent_test.ini\nsetoption name UCI_Variant value transparent_test\nposition fen 4k3/8/8/8/4N3/8/8/4Q2K w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep "Nodes searched")
echo "WITH transparent squares: $transparent_result2"

regular_result2=$(echo -e "setoption name UCI_Variant value chess\nposition fen 4k3/8/8/8/4N3/8/8/4Q2K w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep "Nodes searched")
echo "WITHOUT transparent squares (regular chess): $regular_result2"

echo ""
echo "=== Test completed ==="
