#!/bin/bash

# Color definitions
RED="\033[0;31m"
GREEN="\033[0;32m"
YELLOW="\033[1;33m"
BLUE="\033[0;34m"
RESET="\033[0m"
MAGENTA="\033[35m"

# Paths
INVALID_MAPS_DIR="test_maps/invalid"
VALID_MAPS_DIR="test_maps/valid"
PARSER_PROGRAM="./tests/parser"

# Texture paths - change these to your actual texture paths
NORTH_TEXTURE="./assets/north_wall.xpm"
SOUTH_TEXTURE="./assets/south_wall.xpm"
WEST_TEXTURE="./assets/west_wall.xpm"
EAST_TEXTURE="./assets/east_wall.xpm"

# Create directories if they don't exist
mkdir -p "$INVALID_MAPS_DIR"
mkdir -p "$VALID_MAPS_DIR"

# Test results counters
WARN_TESTS=0
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0

# Check if the -f or --failed-only flag is present
# Default: hide successful tests
SHOW_SUCCESSFUL=0

# Check if the -a or --all flag is present
for arg in "$@"; do
  if [ "$arg" == "-a" ] || [ "$arg" == "--all" ]; then
    SHOW_SUCCESSFUL=1
  fi
done

# Function to print header
print_header() {
    echo -e "\n${BLUE}================================================${RESET}"
    echo -e "${BLUE}          CubE 3D Map Parser Test Suite          ${RESET}"
    echo -e "${BLUE}================================================${RESET}\n"
}

# Function to run a test with a specific map
run_test() {
    local map_file=$1
    local expected_exit_code=$2
    local test_type=$3
    ((TOTAL_TESTS++))
    
    # Run with Valgrind and capture its output
    VALGRIND_OUTPUT=$(valgrind --leak-check=full --show-leak-kinds=all --errors-for-leak-kinds=all --error-exitcode=99 $PARSER_PROGRAM "$map_file" 2>&1)
    local actual_exit_code=$?
    
    # Check for memory leaks in Valgrind output
    if echo "$VALGRIND_OUTPUT" | grep -q "ERROR SUMMARY: 0 errors"; then
        local has_leaks=0
    else
        local has_leaks=1
    fi
    
    # Handle error code differently if Valgrind detected errors
    if [ $actual_exit_code -eq 99 ]; then
        actual_exit_code=1  # Normalize to error
    fi
    
    # Determine test status
    local is_success=0
    if [ $actual_exit_code -eq $expected_exit_code ] && [ $has_leaks -eq 0 ]; then
        is_success=1
        ((PASSED_TESTS++))
        echo -n "."  # Print a dot to show progress for successful test
    elif [ $actual_exit_code -eq $expected_exit_code ] && [ $has_leaks -eq 1 ]; then
        ((WARN_TESTS++))
        echo -n "W"  # Print W for warnings (leaks)
    else
        ((FAILED_TESTS++))
    fi
    
    # If hiding successful tests and this test succeeded, skip detailed output
    if [ $SHOW_SUCCESSFUL -eq 0 ] && [ $is_success -eq 1 ]; then
        return
    fi
    
    # For failures, warnings, or when showing all tests, print detailed output
    echo -e "\n${YELLOW}Testing map:${RESET} $map_file"
    echo -e "${YELLOW}Expected exit code:${RESET} $expected_exit_code"
    echo -e "${YELLOW}Actual exit code:${RESET} $actual_exit_code"
    
    if [ $has_leaks -eq 0 ]; then
        echo -e "${GREEN}✓ No memory leaks detected${RESET}"
    else
        echo -e "${RED}✗ Memory leaks detected${RESET}"
        echo -e "${RED}Valgrind leak summary:${RESET}"
        echo "$VALGRIND_OUTPUT" | grep -A 1 "LEAK SUMMARY" | tail -2
    fi
    
    if [ $actual_exit_code -eq $expected_exit_code ]; then
        if [ $has_leaks -eq 0 ]; then
            echo -e "${GREEN}✓ Test PASSED${RESET}"
        else
            echo -e "${YELLOW}⚠ Warning: Test passes functionally but has memory leaks${RESET}"
        fi
    else
        echo -e "${RED}✗ Test FAILED${RESET}"
        echo -e "  ${RED}$test_type map should exit with code $expected_exit_code, but got $actual_exit_code${RESET}"
    fi
    echo -e "------------------------------------------------"
}

# Function to run all invalid map tests
run_invalid_tests() {
    echo -ne "${BLUE}Running Invalid Map Tests...${RESET}\n"

    # Check if there are any maps to test
    if [ ! "$(ls -A $INVALID_MAPS_DIR)" ]; then
        echo -e "${YELLOW}No invalid maps found in $INVALID_MAPS_DIR${RESET}"
        return
    fi

    # Run tests for all maps in the invalid directory
    for map_file in "$INVALID_MAPS_DIR"/*; do
        if [ -f "$map_file" ]; then
            run_test "$map_file" 1 "Invalid"
        fi
    done
    echo
}

# Function to run all valid map tests
run_valid_tests() {
    echo -e "${BLUE}Running Valid Map Tests...${RESET}"

    # Check if there are any maps to test
    if [ ! "$(ls -A $VALID_MAPS_DIR)" ]; then
        echo -e "${YELLOW}No valid maps found in $VALID_MAPS_DIR${RESET}"
        return
    fi

    # Run tests for all maps in the valid directory
    for map_file in "$VALID_MAPS_DIR"/*; do
        if [ -f "$map_file" ]; then
            run_test "$map_file" 0 "Valid"
        fi
    done
    echo
}

# Function to create example invalid maps for testing
create_example_invalid_maps() {
    # echo -e "${BLUE}Creating example invalid maps...${RESET}"

    # 1. Map with missing wall
    cat > "$INVALID_MAPS_DIR/missing_wall.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
EA $EAST_TEXTURE

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
10000000001100000111N0001
1000000000110000001000001
1000000000000000001000001
1100000111010000001000001
1000000000110000001000001
1111111111110000001000001
1111111111111111111111011
EOF

    # 2. Map with invalid color
    cat > "$INVALID_MAPS_DIR/invalid_color.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
EA $EAST_TEXTURE

F 220,100,0
C 225,300,0  

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
10000000001100000111N0001
1000000000110000001000001
1000000000000000001000001
1100000111010000001000001
1000000000110000001000001
1111111111110000001000001
1111111111111111111111111
EOF

    # 3. Map with missing texture
    cat > "$INVALID_MAPS_DIR/missing_texture.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
# Missing EA texture

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
10000000001100000111N0001
1000000000110000001000001
1000000000000000001000001
1100000111010000001000001
1000000000110000001000001
1111111111110000001000001
1111111111111111111111111
EOF

    # 4. Map with multiple players
    cat > "$INVALID_MAPS_DIR/multiple_players.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
EA $EAST_TEXTURE

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
10000000001100000111N0001
1000000000110000001000001
1000000000000000001000001
1100000111010000001000001
1000000000110000001S00001
1111111111110000001000001
1111111111111111111111111
EOF

    # 5. Map with invalid characters
    cat > "$INVALID_MAPS_DIR/invalid_characters.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
EA $EAST_TEXTURE

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
10000000001100000111N0001
1000000000110000001000001
1000X00000000000001000001
1100000111010000001000001
1000000000110000001000001
1111111111110000001000001
1111111111111111111111111
EOF

    # 6. Empty map
    cat > "$INVALID_MAPS_DIR/empty.cub" << EOF
EOF

    # 7. Map with non-enclosed player
    cat > "$INVALID_MAPS_DIR/non_enclosed_player.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
EA $EAST_TEXTURE

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
101100000111000000000000N
1001000000000000000000001
1111111110110000011100001
1000000000110000011100001
1000000000110000001000001
1000000000000000001000001
1100000111010000001000001
1000000000110000001000001
1111111111110000001000001
1111111111111111111111111
EOF
    echo -e "${GREEN}Created example invalid map files in $INVALID_MAPS_DIR${RESET}"
}

# Function to create example valid maps for testing
create_example_valid_maps() {
    # echo -e "${BLUE}Creating example valid maps...${RESET}"

    # 1. Basic valid map
    cat > "$VALID_MAPS_DIR/valid_basic.cub" << EOF
NO $NORTH_TEXTURE
SO $SOUTH_TEXTURE
WE $WEST_TEXTURE
EA $EAST_TEXTURE

F 220,100,0
C 225,30,0

1111111111111111111111111
1000000000110000000000001
1011000001110000000000001
1001000000000000000000001
1111111110110000011100001
10000000001100000111N0001
1000000000110000001000001
1000000000000000001000001
1100000111010000001000001
1000000000110000001000001
1111111111110000001000001
1111111111111111111111111
EOF

    echo -e "${GREEN}Created example valid map files in $VALID_MAPS_DIR${RESET}"
}

# Function to print test summary
print_summary() {
    echo -e "\n${BLUE}================================================${RESET}"
    echo -e "${BLUE}                 Test Summary                  ${RESET}"
    echo -e "${BLUE}================================================${RESET}"
    echo -e "${YELLOW}Total tests:${RESET} $TOTAL_TESTS"
    echo -e "${GREEN}Passed tests:${RESET} $PASSED_TESTS"
    echo -e "${YELLOW}Warnings (passed with leaks): $WARN_TESTS${RESET}"
    echo -e "${RED}Failed tests:${RESET} $FAILED_TESTS"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}All tests passed! 🎉${RESET}"
    else
        echo -e "\n${RED}Some tests failed. Please review the results.${RESET}"
    fi
}

# Main function
main() {
    # Check if parser program exists
    if [ ! -f "$PARSER_PROGRAM" ]; then
        echo -e "${RED}Error: Parser program not found at $PARSER_PROGRAM${RESET}"
        echo -e "${YELLOW}Please compile the parser first with: make parser${RESET}"
        exit 1
    fi

    print_header
    
    # Display texture paths being used
    echo -e "${BLUE}Using the following texture paths:${RESET}"
    echo -e "${YELLOW}North texture:${RESET} $NORTH_TEXTURE"
    echo -e "${YELLOW}South texture:${RESET} $SOUTH_TEXTURE"
    echo -e "${YELLOW}West texture:${RESET} $WEST_TEXTURE"
    echo -e "${YELLOW}East texture:${RESET} $EAST_TEXTURE"
    echo -e "${MAGENTA}Note:${RESET} Update these paths in the script if needed.\n"
    
    # Create example maps if needed
    create_example_invalid_maps
    create_example_valid_maps
    
    # Run tests
    run_invalid_tests
    run_valid_tests
    
    # Print summary
    print_summary

    # Return exit code based on test results
    if [ $FAILED_TESTS -gt 0 ]; then
        exit 1
    else
        exit 0
    fi
}

# Execute main function
main