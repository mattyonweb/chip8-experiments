#include <ncurses.h>
#include <unistd.h>

#define DELAY 30000

int main(int argc, char *argv[]) {
 int x = 0, y = 0;

 initscr();
 noecho();
 curs_set(FALSE);

 while(1) {
 clear(); // Clear the screen of all
 // previously-printed characters
 mvprintw(y, x, "o"); // Print our "ball" at the current xy position
 refresh();

 usleep(DELAY); // Shorter delay between movements
 x++; // Advance the ball to the right
 }

 endwin();
}
