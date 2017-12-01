#include <ncurses.h>

#include <vector>
#include <algorithm>

#include <ctime>
#include <cmath>
#include <unistd.h>

using namespace std;

const int rocket_interval = 80000;
const int letter_interval = 100000;
int rand_between(int max_, int min_) { return rand() % (max_ - min_) + min_; }


class Rocket {
	int pos_y;
	int pos_x;
	int win_y;
	int win_x;
	int burst;
	int size;
	int count_;
	bool up;
	
	public:
	Rocket(int win_y, int win_x) : win_y(win_y), win_x(win_x),
			burst(0), up(false) {
		pos_y = win_y;
		pos_x = rand_between(win_x-10, 10);
		size = rand() % 13 + 7;
		count_ = 0;
	}
	void draw() {
		if (!burst) {
			if (pos_y < win_y) mvaddch(pos_y, pos_x, 'A');
			if (pos_y+1 < win_y) mvaddch(pos_y+1, pos_x, '|');
			if (pos_y+2 < win_y) mvaddch(pos_y+2, pos_x, '|');
			if (pos_y+3 < win_y) mvaddch(pos_y+3, pos_x, '*');
			if (pos_y+4 < win_y) mvaddch(pos_y+4, pos_x, '*');
			if (pos_y+5 < win_y) mvaddch(pos_y+5, pos_x, '*');
		} else if (burst < size) {
			int x, y;
			char ch = (burst % 2) ? '#' : '*';
			for (float deg=0; deg < 360.0; ++deg) {
				float rad = deg * 3.1416 / 180.0;
				y = pos_y + (burst * sin(rad));
				x = pos_x + (burst * cos(rad));
				mvaddch(y, x, ch);
			}
		}
	}
	void update() {
		if (!burst) {
			int prob = rand() % 100;
			if (pos_y < 0.20 * win_y) burst++;
			else if (pos_y < 0.25 * win_y && prob < 75) burst++;
			else if (pos_y < 0.30 * win_y && prob < 50) burst++;
			else if (pos_y < 0.35 * win_y && prob < 25) burst++;
			else pos_y--;
		}
		else if (burst < size) {
			burst += (up) ? 1 : 0;
			up = !up;
		}
	}
	bool bursted() const {
		return (burst == size);
	}
};


void draw_H(int y, int x)
{
	mvaddch(y, x, '|');
	mvaddch(y+1, x, '|');
	mvaddch(y+2, x, '|');
	mvaddch(y+3, x, '|');
	
	mvaddch(y, x+3, '|');
	mvaddch(y+1, x+3, '|');
	mvaddch(y+2, x+3, '|');
	mvaddch(y+3, x+3, '|');
	
	mvaddch(y+1, x+1, '_');
	mvaddch(y+1, x+2, '_');
}

void draw_A(int y, int x)
{
	mvaddch(y, x+1, '/');
	mvaddch(y+1, x, '|');
	mvaddch(y+2, x, '|');
	mvaddch(y+3, x, '|');
	
	mvaddch(y, x+2, '\\');
	mvaddch(y+1, x+3, '|');
	mvaddch(y+2, x+3, '|');
	mvaddch(y+3, x+3, '|');
	
	mvaddch(y+1, x+1, '_');
	mvaddch(y+1, x+2, '_');
}

void draw_P(int y, int x)
{
	mvaddch(y, x, '|');
	mvaddch(y+1, x, '|');
	mvaddch(y+2, x, '|');
	mvaddch(y+3, x, '|');
	
	mvaddch(y-1, x+1, '_');
	mvaddch(y, x+2, '\\');
	mvaddch(y+1, x+3, '|');
	mvaddch(y+1, x+2, '_');
	mvaddch(y+1, x+1, '_');
}

void draw_Y(int y, int x)
{
	mvaddch(y, x, '\\');
	mvaddch(y+1, x+1, '\\');
	mvaddch(y, x+3, '/');
	mvaddch(y+1, x+2, '/');
	mvaddch(y+2, x+1, '/');
	mvaddch(y+3, x, '/');
}

void draw_D(int y, int x)
{
	mvaddch(y, x, '|');
	mvaddch(y+1, x, '|');
	mvaddch(y+2, x, '|');
	mvaddch(y+3, x, '|');
	
	mvaddch(y-1, x+1, '_');
	mvaddch(y, x+2, '\\');
	mvaddch(y+1, x+3, '|');
	mvaddch(y+2, x+3, '|');
	mvaddch(y+3, x+2, '/');
	mvaddch(y+3, x+1, '_');
}

void draw_I(int y, int x)
{
	mvaddch(y, x+1, '|');
	mvaddch(y+1, x+1, '|');
	mvaddch(y+2, x+1, '|');
	mvaddch(y+3, x+1, '|');
	
	mvaddch(y-1, x, '_');
	mvaddch(y-1, x+2, '_');
	mvaddch(y+3, x, '_');
	mvaddch(y+3, x+2, '_');
}

void draw_W(int y, int x)
{
	mvaddch(y, x, '|');
	mvaddch(y+1, x, '|');
	mvaddch(y+2, x, '|');
	mvaddch(y+3, x, '|');
	
	mvaddch(y, x+3, '|');
	mvaddch(y+1, x+3, '|');
	mvaddch(y+2, x+3, '|');
	mvaddch(y+3, x+3, '|');
	
	mvaddch(y+3, x+1, '/');
	mvaddch(y+3, x+2, '\\');
}

void draw_L(int y, int x)
{
	mvaddch(y, x, '|');
	mvaddch(y+1, x, '|');
	mvaddch(y+2, x, '|');
	mvaddch(y+3, x, '|');
	
	mvaddch(y+3, x+1, '_');
	mvaddch(y+3, x+2, '_');
}


int main()
{
	// Setup.
	srand(time(NULL));
	initscr();
	curs_set(0);
	
	// Window sizes.
	int win_y, win_x;
	getmaxyx(stdscr, win_y, win_x);

	vector<Rocket> rocks;
	rocks.push_back(Rocket(win_y, win_x));

	// Initial random rockets.
	unsigned count = 1;
	while (rocks.size() != 0) {
		if (count < 10) {
			int prob = rand() % 100;
			if (prob < 35) {
				count++;
				rocks.push_back(Rocket(win_y, win_x));
			}
		}
		for (Rocket& r : rocks) {
			r.update();
			r.draw();
		}
		refresh();
		usleep(rocket_interval);
		clear();
		rocks.erase(remove_if(rocks.begin(), rocks.end(),
			[] (const Rocket& r) {return r.bursted();}), rocks.end());
	}

	usleep(rocket_interval);
	
	// Happy Diwali.
	int text_y = win_y/2;
	int text_x = win_x/2 - 24;

	for (char ch: "HAPPY DIWALI") {
		switch(ch) {
			case 'H': draw_H(text_y, text_x); break;
			case 'A': draw_A(text_y, text_x); break;
			case 'P': draw_P(text_y, text_x); break;
			case 'Y': draw_Y(text_y, text_x); break;
			case 'D': draw_D(text_y, text_x); break;
			case 'I': draw_I(text_y, text_x); break;
			case 'W': draw_W(text_y, text_x); break;
			case 'L': draw_L(text_y, text_x); break;
		}
		usleep(letter_interval);
		refresh();
		text_x += 4;
	}

	getch();
	endwin();
	return 0;
}
