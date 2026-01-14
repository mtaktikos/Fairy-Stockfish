#!/bin/bash
# Test transparent squares feature

cd "$(dirname "$0")/../src"

# Test 1: Load the transparent_test variant
echo "Test 1: Load transparent_test variant"
echo -e "load ../tests/transparent_test.ini\nsetoption name UCI_Variant value transparent_test\nposition startpos\nd\nquit" | ./stockfish 2>&1 | grep -A30 "Checkers:"

# Test 2: Set up a position with a knight (transparent piece) and check if sliders can move through
echo ""
echo "Test 2: Position with knight at d4 - rook should be able to slide through transparent squares"
echo -e "load ../tests/transparent_test.ini\nsetoption name UCI_Variant value transparent_test\nposition fen 4k3/8/8/8/3N4/8/4R3/4K3 w - - 0 1\nd\nquit" | ./stockfish 2>&1 | grep -A30 "Checkers:"

echo ""
echo "All tests completed"
