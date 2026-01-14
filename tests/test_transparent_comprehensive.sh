#!/bin/bash
# Detailed test for transparent squares feature

cd "$(dirname "$0")/../src"

echo "=== Testing Transparent Squares Feature ==="
echo ""

# Helper function to run a test
run_test() {
    local test_name="$1"
    local fen="$2"
    echo "Test: $test_name"
    echo "FEN: $fen"
    
    result=$(echo -e "load ../tests/transparent_test.ini\nsetoption name UCI_Variant value transparent_test\nposition fen $fen\ngo perft 1\nquit" | ./stockfish 2>&1 | grep "Nodes searched")
    echo "Result: $result"
    echo ""
}

# Test 1: White rook with white Witch creating transparent squares
run_test "White rook with white Witch (W, transparent piece)" "4k3/8/8/8/4W3/8/8/4R2K w - - 0 1"

# Test 2: Black rook with black witch creating transparent squares  
run_test "Black rook with black witch (w, transparent piece)" "4r2k/8/8/8/4w3/8/8/4K3 b - - 0 1"

# Test 3: Standard starting position
run_test "Standard starting position with Witch" "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKWNR w KQkq - 0 1"

# Test 4: White bishop with white Witch
run_test "White bishop with white Witch (W, transparent piece)" "4k3/8/8/8/4W3/8/8/B3K3 w - - 0 1"

# Test 5: Rook with friendly piece on adjacent square to Witch
run_test "Rook cannot capture friendly piece on transparent square" "4k3/8/8/4P3/4W3/8/8/4R2K w - - 0 1"

# Test 6: Rook with enemy piece on adjacent square to Witch
run_test "Rook CAN capture enemy piece on transparent square" "4k3/8/8/4p3/4W3/8/8/4R2K w - - 0 1"

# Test 7: Queen sliding through multiple transparent squares
run_test "Queen with Witch creating multiple transparent squares" "4k3/8/8/8/4W3/8/8/4Q2K w - - 0 1"

echo "=== All tests completed ==="
