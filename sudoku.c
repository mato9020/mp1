#include <stdio.h>
#include <stdbool.h>    //allows for bool type

#define GRIDSIZE  3
#define GRID_SQUARED  GRIDSIZE*GRIDSIZE
#define ALL_VALUES  (1<<GRID_SQUARED)-1     //0b111111111
const gridsize = 3;

int board[GRID_SQUARED][GRID_SQUARED];

char *easy_board_init[GRID_SQUARED] = {"******3**", 
													"*8***3**9", 
													"*24*7**86", 
													"9****54*8", 
													"***4*2***", 
													"1*69****5", 
													"56**3*14*", 
													"4**5***9*", 
													"**7******"};

char *hard_board_init[GRID_SQUARED] = {"*94******", 
													"1**79****", 
													"**3*86**1", 
													"92*****3*", 
													"****7****", 
													"*8*****25", 
													"4**52*9**", 
													"****61**3", 
													"******64*"};

int bit_count(int value);
int get_nth_set_bit(int value, int index);
bool singleton(int value); 
int get_singleton(int value);
bool rule2();

// Use the "board_init" variable to initialize the bitmasks for each square.
void
init_board(char *board_init[GRID_SQUARED]) {
  for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
	 for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
		char c = board_init[i][j];
		if (c == '*') {
		  board[i][j] = ALL_VALUES;
		} else {
		  board[i][j] = 1 << (c - '0' - 1);  //  DO YOU UNDERSTAND WHAT THIS DOES?  YOU SHOULD. This sets the bit in the correct spot... if c = '3' it puts 0b0100 there.
		}
	 }
  }
}

// A board is done if all squares are singletons.
bool
board_done(int board[GRID_SQUARED][GRID_SQUARED]) {
  for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
	 for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
		if (!singleton(board[i][j])) {
		  return false;
		}
	 }
  }
  return true;
}

// Print the board with one value per square.
void
print_board(int board[GRID_SQUARED][GRID_SQUARED]) {
  for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
	 for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
		int value = board[i][j];
		char c = '*';
		if (singleton(value)) {
		  c = get_singleton(value) + '1';
		}
		printf("%c", c);
	 }
	 printf("\n");
  }
}

// Print the board with up to 9 values per square, based on what
// the possibilities are.
void
print_board_verbose() {
  for (int i = 0 ; i < GRID_SQUARED ; ++ i) {
	 for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
		int value = board[i][j];
		char c = ' ';
		if (singleton(value)) {
		  c = '*';
		}
		for (int k = 0 ; k < GRID_SQUARED ; ++ k) {
		  char cc = (value & (1<<k)) ? ('1'+k) : c;
		  printf("%c", cc);
		}
		printf("%c", (j == (GRID_SQUARED-1)) ? '\n' : '|');
	 }
  }
}

int 
get_square_begin(int index) {
  return (index/GRIDSIZE) * GRIDSIZE;
}

bool
rule1() {
  bool changed = false;
  for (int i = 0 ; i < GRID_SQUARED ; ++ i) {  
	 for (int j = 0 ; j < GRID_SQUARED ; ++ j) {
		// for every square, if the value is a singleton, remove it from the row, column, and square.
		int value = board[i][j];
		if (singleton(value)) {
		  for (int k = 0 ; k < GRID_SQUARED ; ++ k) {
			 // eliminate from row
			 if (k != j) {
				if (board[i][k] & value) {
				  changed = true;
				}
				board[i][k] &= ~value;
			 }
			 // eliminate from column
			 if (k != i) {
				if (board[k][j] & value) {
				  changed = true;
				}
				board[k][j] &= ~value;
			 }
		  }

		  // elimnate from square
		  int ii = get_square_begin(i);
		  int jj = get_square_begin(j);
		  for (int k = ii ; k < ii + GRIDSIZE ; ++ k) {
		  	 for (int l = jj ; l < jj + GRIDSIZE ; ++ l) {
		  		if ((k == i) && (l == j)) {
		  		  continue;
		  		}
				if (board[k][l] & value) {
				  changed = true;
				}
		  		board[k][l] &= ~value;
		  	 }
		  }
		}
	 }
  }
  return changed;
}

bool solve(char *board_init[GRID_SQUARED]) {
    init_board(board_init);
    bool changed;
    do {
	    changed = rule1();
	    changed |= rule2();
    } while (changed);
    if (board_done(board)) {
	     printf("SUCCESS!\n");
	    print_board(board);
	    printf("\n");
	    return true;
    } else {
	    printf("FAILED!\n");
	    print_board_verbose();
	    printf("\n");
	    return false;
    }
}

int
main() {
    printf("bit_count(0x34) = %d\n", bit_count(0x34));
    printf("get_nth_set_bit(0x34, 1) = %d\n", get_nth_set_bit(0x34, 1));
    printf("singleton(0x40) = %s\n", singleton(0x40) ? "TRUE" : "FALSE");
    printf("get_singleton(0x40) = %d\n\n", get_singleton(0x40));

    printf("EASY BOARD:\n");
    bool solved = solve(easy_board_init);
    if (solved) {
	    printf("HARD BOARD:\n");
	    solve(hard_board_init);
    }
    return 0;
}

// ALL your code goes below this line.
// 
// ---------------------------------------------------------------------
int bit_count(int value) {
    int counter = 0;
    for (int i = 0; i < 8*sizeof(value); i++){
        if ((value >> i) & 1 == 1){
            counter++;
        }
    }
    return counter;
}

int get_nth_set_bit(int value, int index) {
    int counter = -1;
    if (bit_count(value) - 1 < index){
        return -1;
    }
    for(int i = 0; i < 8*sizeof(value); i++){
        if ((value >> i) & 1 == 1){
            counter++;
            if(counter == index){
                return i;
            }
        }
    }
    return -1;
}

bool singleton(int value) {  
    if (value == 0){
        return 0;
    }
    else{
        if(((value-1) & value) == 0){
            return true;
        }
        else{
            return false;
        }
    }
}

int get_singleton(int value) {
    if (singleton(value) == 1){
        for (int i = 0; i < 8*sizeof(value); i++){
            if ((value >> i) & 1 == 1){
                return i;
            }
        }
    }
    else{
        return -1;
    }
}

bool rule2() {
  bool changed = false;
  for (int i = 0; i < 9; i++){
      for (int j = 0; j < 9; j++){
          int value = board[i][j];
          if (!singleton(value)){
              int isum = 0;
              for (int k = 0; k < 9; k++){
                  if (k != i){
                      isum |= board[k][j]; 
                  }
              }
              if (isum != ALL_VALUES){
                  board[i][j] = ~isum & 0b111111111;
                  changed = true;
              }
              int jsum = 0;
              for (int k = 0; k < 9; k++){
                  if (k != j){
                      jsum |= board[i][k];
                  }
              }
              if (jsum != ALL_VALUES){
                  board[i][j] = ~jsum & 0b111111111;
                  changed = true;
              }              
              int ksum = 0;
              int ii = get_square_begin(i);
              int jj = get_square_begin(j);
              for (int k = ii; k < ii + 3; k++){
                  for (int l = jj; l < jj + 3; l++){
                      if ((k == i) && (l == j)){
                          continue;
                      }
                      else{
                          ksum |= board[k][l];
                      }
                  }
              }
              
              if (ksum != ALL_VALUES){
                  board[i][j] = ~ksum & 0b111111111;
                  changed = true;
              }
          }
      }
  }
  return changed;
}
