#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS

#include<math.h>
#include<stdio.h>
#include<string.h>
#include<time.h>
#include<ctime>
#include<cstdlib>

extern "C" {
#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"
}

#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

#define numberofrectangles 4
#define smallrectanglesize 90

/*Beginning of GLOBAL VARIABLES DECLARATIONS*/

//actual position of tiles on screen
int actual[numberofrectangles][numberofrectangles];
//last position of tiles on screen
int lastmove[numberofrectangles][numberofrectangles];
//an intermediate array when moving actual to lastmove
int possible[numberofrectangles][numberofrectangles];
int semi[numberofrectangles][numberofrectangles];
//variable storing actual number of points
int points;
//variable storing the lost status (1-no moves)
bool gameover;

/*End of GLOBAL VARIABLES DECLARATIONS*/

// draw a text txt on surface screen, starting from the point (x, y)
// charset is a 128x128 bitmap containing character images
void DrawString(SDL_Surface *screen, int x, int y, const char *text,
                SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while(*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
		};
	};

// draw a surface sprite on a surface screen in point (x, y)
// (x, y) is the center of sprite on screen
void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
	};

// draw a single pixel
void DrawPixel(SDL_Surface *surface, int x, int y, Uint32 color) {
	int bpp = surface->format->BytesPerPixel;
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;
	*(Uint32 *)p = color;
	};

// draw a vertical (when dx = 0, dy = 1) or horizontal (when dx = 1, dy = 0) line
void DrawLine(SDL_Surface *screen, int x, int y, int l, int dx, int dy, Uint32 color) {
	for(int i = 0; i < l; i++) {
		DrawPixel(screen, x, y, color);
		x += dx;
		y += dy;
		};
	};

// draw a rectangle of size l by k
void DrawRectangle(SDL_Surface *screen, int x, int y, int l, int k,
                   Uint32 outlineColor, Uint32 fillColor) {
	int i;
	DrawLine(screen, x, y, k, 0, 1, outlineColor);
	DrawLine(screen, x + l - 1, y, k, 0, 1, outlineColor);
	DrawLine(screen, x, y, l, 1, 0, outlineColor);
	DrawLine(screen, x, y + k - 1, l, 1, 0, outlineColor);
	for(i = y + 1; i < y + k - 1; i++)
		DrawLine(screen, x + 1, i, l - 2, 1, 0, fillColor);
	};

//start new game
void newgame(double* worldtime, double* finishtime, bool* bul)
{
	//zero all variable
	points = 0;
	gameover = false;
	int y = rand() % 4;
	int x = rand() % 4;
	*worldtime = 0;
	*finishtime = 0;
	*bul = false;
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			//put zero in each rectangle 
			actual[i][j] = 0;
			lastmove[i][j]=0;
		}
	}
	actual[y][x] = 2;
};

//store last move in table lastmove[][]
void savelastmove()
{
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			lastmove[i][j] = semi[i][j];
		}
	}
};

//check if we are able to do next move 1-no 0-yes
bool isgameover()
{
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles-1; j++)
		{	
			//chech if we can connect rectangles, than return true
			if (actual[i][j] == actual[i][j + 1] || actual[j][i] == actual[j + 1][i])
			{
				
					return true;
			}
		}
	}
	// if any connection could have benn found return false - game over 
	return false;
};

//add new block in random place 
void aftermove()
{
	int emptynumber = 0;
	bool empty = false;
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			if (actual[i][j] == 0)
			{
				empty = true;
				emptynumber++;
			}
		}
	}
	bool zmienna = isgameover();
	if (!zmienna)
	{
		gameover = 1;
	}
	if (!empty)
	{
	}
	else
	{
		savelastmove();
		bool brejk = 0;
		int y = rand() % emptynumber;
		for (int i = 0; i < numberofrectangles; i++)
		{
			for (int j = 0; j < numberofrectangles; j++)
			{
				if (actual[i][j] == 0)
				{
					if (y == 0)
					{
						brejk = 1;
						actual[i][j] = 2;
					}
					else
					{
						y--;
					}
				}
				if (brejk == 1)
				{
					break;
				}
			}
			if (brejk == 1)
			{
				break;
			}
		}
	}
	
};

//move of block - left
void moveleft()
{
	bool move = 0;
	int p;
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 1; j < numberofrectangles; j++)
		{
			if (actual[j][i] != 0)
			{
				int pomocnicza = 0;
				 p = 1;
				while (p)
				{
					//check if we are able to mova left on this position... 0 or the same number on left side
					if ((actual[pomocnicza][i] == 0 || actual[pomocnicza][i] == actual[j][i]) 
						&& ((actual[pomocnicza+1][i] == 0)||(pomocnicza+1 == j)))
					{
						if (actual[pomocnicza][i] == actual[j][i])
						{
							points = points + actual[pomocnicza][i] + actual[j][i];
						}
						actual[pomocnicza][i] += actual[j][i];
						actual[j][i] = 0;
						p = 0;
						move = 1;
						possible[pomocnicza][i] = 1;
					}
					pomocnicza++;
					if (pomocnicza == j)
					{
						p = 0;
					}
					
				}
			}
		}
	}
	if (move == 1)
	{
		aftermove();
	}

};

//move of block - right
void moveright()
{
	bool move = 0;
	int p;
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = numberofrectangles - 2; j >= 0; j--)
		{
			if (actual[j][i] != 0)
			{
				int pomocnicza = numberofrectangles - 1;
				p = 1;
				while (p)
				{
					//check if we are able to mova left on this position... 0 or the same number on right side
					if ((actual[pomocnicza][i] == 0 || actual[pomocnicza][i] == actual[j][i]) 
						&& ((actual[pomocnicza - 1][i] == 0) || (pomocnicza - 1== j)))
					{
						if (actual[pomocnicza][i] == actual[j][i])
						{
							points = points + actual[pomocnicza][i] + actual[j][i];
						}
						actual[pomocnicza][i] += actual[j][i];
						actual[j][i] = 0;
						p = 0;
						move = 1;
						possible[pomocnicza][i] = 1;
					}
					pomocnicza--;
					if (pomocnicza == j)
					{
						p = 0;
					}

				}
			}
		}
	}
	if (move == 1)
	{
		aftermove();
	}

};

//move of block - up
void moveup()
{
	bool move = 0;
	int p;
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 1; j < numberofrectangles; j++)
		{
			if (actual[i][j] != 0)
			{
				int pomocnicza = 0;
				p = 1;
				while (p)
				{
					//check if we are able to mova left on this position... 0 or the same number up
					if ( (actual[i][pomocnicza] == 0 || actual[i][pomocnicza] == actual[i][j])
						&& ((actual[i][pomocnicza + 1] == 0) || (pomocnicza + 1 == j)))
					{
						if (actual[i][pomocnicza] == actual[i][j])
						{
							points = points + actual[i][pomocnicza] + actual[i][j];

						}
						actual[i][pomocnicza] += actual[i][j];
						actual[i][j] = 0;
						p = 0;
						move = 1;
						possible[i][pomocnicza] = 1;
					}
					pomocnicza++;
					if (pomocnicza == j)
					{
						p = 0;
					}
					
				}
			}
		}
	}
	if (move == 1)
	{
		aftermove();
	}
};

//move of block - down
void movedown()
{
	bool move = 0;
	int p;
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = numberofrectangles - 2; j >= 0; j--)
		{
			if (actual[i][j] != 0)
			{
				int pomocnicza = numberofrectangles - 1;
				p = 1;
				while (p)
				{
					//check if we are able to mova left on this position... 0 or the same number down
					if ((actual[i][pomocnicza] == 0 || actual[i][pomocnicza] == actual[i][j]) &&
						((actual[i][pomocnicza - 1] == 0) || (pomocnicza - 1 == j)))
					{
						if (actual[i][pomocnicza] == actual[i][j])
						{
							points = points + actual[i][pomocnicza] + actual[i][j];
						}
						actual[i][pomocnicza] += actual[i][j];
						actual[i][j] = 0;
						p = 0;
						move = 1;
						possible[i][pomocnicza] = 1;
					}
					pomocnicza--;
					if (pomocnicza == j)
					{
						p = 0;
					}

				}
			}
		}
	}
	if (move == 1)
	{
		aftermove();
	}
};

//checking if we've won 1-true 0-false
bool isend()
{
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			//there we can choose when we want to finish game
			if (actual[i][j] == 2048)
			{
				return true;
			}
		}
	}
	return false;
};

//print board with numbers on screen
void printplansza(SDL_Surface *screen, SDL_Surface *charset, int color2, int color4, int color8, int color16, 
	int color32, int color64, int color128, int color256, int color512, int color1024, int color2048)
{
	int color = 0;
	char text[128];
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			if (actual[i][j] != 0)
			{
				int punkt = actual[i][j];
			
				//using switch case for changing color of rectangle depending on points
				switch (punkt)
				{
				case 2:
					color = color2;
					break;
				case 4:
					color = color4;
					break;
				case 8:
					color = color8;
					break;
				case 16:
					color = color16;
					break;
				case 32:
					color = color32;
					break;
				case 68:
					color = color64;
					break;
				case 128:
					color = color128;
					break;
				case 256:
					color = color256;
					break;
				case 512:
					color = color512;
					break;
				case 1024:
					color = color1024;
					break;
				default:
					color = color2048;
					break;
				}
				DrawRectangle(screen, (SCREEN_WIDTH / 2) - (2 * smallrectanglesize) + (i * smallrectanglesize) + 1, (SCREEN_HEIGHT / 2) - (2 * smallrectanglesize) + 1 + (j*smallrectanglesize), smallrectanglesize - 1, smallrectanglesize - 1, color, color);
				int value =  actual[i][j];
				sprintf(text,"%d", value);
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset); 
				DrawString(screen, (SCREEN_WIDTH / 2) - (2 * smallrectanglesize) + (i * smallrectanglesize) + 1 + (smallrectanglesize/2) , (SCREEN_HEIGHT / 2) - (2 * smallrectanglesize) + 1 + (j*smallrectanglesize) + (smallrectanglesize / 2), text, charset);
			}
		}
	}
};

//checking if move can be done
void savesemi()
{
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			semi[i][j] = actual[i][j];
		}
	}
};

//transfer lastmove[] to actual[] after succesful move
void loadlastmove()
{
	for (int i = 0; i < numberofrectangles; i++)
	{
		for (int j = 0; j < numberofrectangles; j++)
		{
			//transfer data from actual[] to lastmove[]
			 actual[i][j]=lastmove[i][j];
		}
	}
};

// main
#ifdef __cplusplus
extern "C"
#endif
int main(int argc, char **argv) {
	//aktualna postac planszy
	int t1, t2, quit, frames, rc;
	double delta, worldTime, fpsTimer, fps, distance, etiSpeed;
	double finishtime = 0;
	bool bul = false;
	SDL_Event event;
	SDL_Surface *screen, *charset;
	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	srand(time(NULL));

	//initial game reset to start with a clean whiteboard
	newgame(&worldTime, &finishtime, &bul);

	// console window is not visible, to see the printf output
	// the option:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// must be changed to "Console"
	printf("printf output goes here\n");

	if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
		}

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
	                                 &window, &renderer);
	if(rc != 0) {
		SDL_Quit();
		printf("SDL_CreateWindowAndRenderer error: %s\n", SDL_GetError());
		return 1;
		};
	
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	//showing title of window
	SDL_SetWindowTitle(window, "2048 by Lukasz Golojuch");


	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
	                              0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
	                           SDL_TEXTUREACCESS_STREAMING,
	                           SCREEN_WIDTH, SCREEN_HEIGHT);


	// disable the visibility of the mouse cursor
	SDL_ShowCursor(SDL_DISABLE);

	// load picture cs8x8.bmp
	charset = SDL_LoadBMP("./cs8x8.bmp");
	if(charset == NULL) {
		printf("SDL_LoadBMP(cs8x8.bmp) error: %s\n", SDL_GetError());
		SDL_FreeSurface(screen);
		SDL_DestroyTexture(scrtex);
		SDL_DestroyWindow(window);
		SDL_DestroyRenderer(renderer);
		SDL_Quit();
		return 1;
		};
	SDL_SetColorKey(charset, true, 0x000000);

	char text[128];
	int czarny = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int zielony = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	int czerwony = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	int niebieski = SDL_MapRGB(screen->format, 0x11, 0x11, 0xCC);

	//kolors for each number of points (colornumberofpoints)
	int color2 = SDL_MapRGB(screen->format, 0x99, 0x84, 0x46);
	int color4 = SDL_MapRGB(screen->format, 0xE6, 0x84, 0x46);
	int color8 = SDL_MapRGB(screen->format, 0x42, 0xFF, 0x46);
	int color16 = SDL_MapRGB(screen->format, 0xff, 0x84, 0xFF);
	int color32 = SDL_MapRGB(screen->format, 0xE3, 0xCC, 0x00);
	int color64 = SDL_MapRGB(screen->format, 0x02, 0xBB, 0xAA);
	int color128 = SDL_MapRGB(screen->format, 0xFF, 0x00, 0xF0);
	int color256 = SDL_MapRGB(screen->format, 0xFF, 0x70, 0x00);
	int color512 = SDL_MapRGB(screen->format, 0x27, 0xD4, 0xD6);
	int color1024 = SDL_MapRGB(screen->format, 0xBE, 0x78, 0x7C);
	int color2048 = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);

	t1 = SDL_GetTicks();

	frames = 0;
	fpsTimer = 0;
	fps = 0;
	quit = 0;
	worldTime = 0;
	distance = 0;
	etiSpeed = 1;


	while(!quit) {
		t2 = SDL_GetTicks();

		// here t2-t1 is the time in milliseconds since
		// the last screen was drawn
		// delta is the same time in seconds
		delta = (t2 - t1) * 0.001;
		t1 = t2;

		worldTime += delta;

		SDL_FillRect(screen, NULL, czarny);

		//wyswietlanie planszy lub informacji o zwyciestwie i porazce
		if (bul == false && isend() == 1)
		{
			finishtime = worldTime;
			bul = true;
		}

		//firstly we must check if we won in last move 
		if (bul)
		{
			sprintf(text, "Gratulacje wygrales! Twoj wynik to %d punktow", points);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, screen->h / 2 - strlen(text) * 8 / 2, text, charset);
		}
		else
		{
			//secondly we must check if we lose in last move
			if (gameover == 1)
			{
				sprintf(text, "Przegrales");
				DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, screen->h / 2 - strlen(text) * 8 / 2, text, charset);
			}
			else
			{
				//...otherwise we can do next move and continue game
				DrawRectangle(screen, (SCREEN_WIDTH / 2) - (2 * smallrectanglesize), (SCREEN_HEIGHT / 2) - (2 * smallrectanglesize), (4 * smallrectanglesize) + 1, (4 * smallrectanglesize) + 1, czerwony, czarny);
				printplansza(screen, charset, color2, color4, color8, color16, color32, color64, color128, color256, color512, color1024, color2048 );
			}
		}

		fpsTimer += delta;
		if(fpsTimer > 0.5) {
			fps = frames * 2;
			frames = 0;
			fpsTimer -= 0.5;
			};

		//header 
		DrawRectangle(screen, 4, 4, SCREEN_WIDTH - 8, 54, czerwony, niebieski);
		sprintf(text, "2048 by Lukasz Golojuch, time = %.1lf s  ", worldTime);
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

		sprintf(text, "Esc - Exit, n - new game, u - repeat move");
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 26, text, charset);
		//if we haven't lose yet, than show points
		if (gameover == 0)
		{
			sprintf(text, "Points: %d", points);
			DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 42, text, charset);
		}

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);

		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		
		

		//handling of events (if there were any)
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_KEYDOWN:
					if(event.key.keysym.sym == SDLK_ESCAPE) quit = 1; //press esc to exit the game
					else if(event.key.keysym.sym == SDLK_UP) savesemi(), moveup(); //press up to move up
					else if(event.key.keysym.sym == SDLK_DOWN)savesemi(), movedown(); //press down to move down 
					else if (event.key.keysym.sym == SDLK_RIGHT)savesemi(), moveright(); //press right to move right
					else if (event.key.keysym.sym == SDLK_LEFT) savesemi(), moveleft(); //press left to move left
					else if (event.key.keysym.sym == SDLK_n) newgame(&worldTime, &finishtime, &bul); //press n to start new game
					else if (event.key.keysym.sym == SDLK_u) loadlastmove(); //press u to go back to last move
					break;
				case SDL_QUIT:
					quit = 1;
					break;
			};
			};
		frames++;
		};

	//freeing all surfaces
	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
	};
