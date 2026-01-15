#!/bin/bash
# Test transparent pawn double-push feature

cd "$(dirname "$0")/../src"

echo "=========================================="
echo "Test 1: White pawn on b2, Witch on b3 (whitetransparent), empty b4"
echo "Expected: Pawn should be able to double-push from b2 to b4"
echo "=========================================="
echo -e "load ../tests/transparent_pawn_test.ini\nsetoption name UCI_Variant value transparent_pawn_test\nposition fen 4k3/8/8/8/8/1W6/1P6/4K3 w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep -A20 "^b2"

echo ""
echo "=========================================="
echo "Test 2: Black pawn on e7, Witch on e6 (blacktransparent), empty e5"
echo "Expected: Pawn should be able to double-push from e7 to e5"
echo "=========================================="
echo -e "load ../tests/transparent_pawn_test.ini\nsetoption name UCI_Variant value transparent_pawn_test\nposition fen 4k3/4p3/4w3/8/8/8/8/4K3 b - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep -A20 "^e7"

echo ""
echo "=========================================="
echo "Test 3: White pawn on a2, regular piece (non-transparent) on a3"
echo "Expected: Pawn should NOT be able to double-push (a3 is not transparent)"
echo "=========================================="
echo -e "load ../tests/transparent_pawn_test.ini\nsetoption name UCI_Variant value transparent_pawn_test\nposition fen 4k3/8/8/8/8/N7/P7/4K3 w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep -A20 "Nodes searched"

echo ""
echo "=========================================="
echo "Test 4: Multiple white pawns with transparent squares"
echo "Expected: b2b4 should be possible (Witch on b3), c2c4 should NOT be possible (Knight on c3)"
echo "=========================================="
echo -e "load ../tests/transparent_pawn_test.ini\nsetoption name UCI_Variant value transparent_pawn_test\nposition fen 4k3/8/8/8/8/1W1N4/1P1P4/4K3 w - - 0 1\ngo perft 1\nquit" | ./stockfish 2>&1 | grep -E "^[a-h][0-9]|Nodes searched"

echo ""
echo "All tests completed"
