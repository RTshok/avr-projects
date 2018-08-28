/**
* That are the variables which are used to move blocks.
* These vars increment, and the blocks are moving.
*/
extern uint8_t x,x1;
/**
* The position of the bird in that moment. Default = 25.
*/
extern uint8_t bird_pos;
/**
* Shows the amount of windows that the bird flew without crashing. Default = 0.
*/
extern uint8_t score;
/**
* The width of the block. Width by x axis.
*/
extern uint8_t width_of_block;
/**
* The height of the window, which the bird is supposed to fly through. By y axis.
*/
extern uint8_t height_of_window;
/**
* Array to contain random numbers for random position of the window on the display.
*/
extern uint8_t RAND[2];
/**
* The array which contains the data-bytes.
* That bytes are used to display the bird on the screen.
*/ 
extern const uint8_t BIRD[];