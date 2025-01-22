#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>

#define ROAD_WIDTH 20
#define ROAD_HEIGHT 20
#define CAR_WIDTH 3
#define CAR_HEIGHT 2
#define OBSTACLE_WIDTH 3
#define OBSTACLE_HEIGHT 2

typedef struct{
 int x, y;
}Position;


/*
TODO

next part 
1. Add a replay section where people can press "r" to replay the game and compete with their own score if they beat their score then the score can be overwritten.
2. Add a prompt to the user to select which type of car they want [ user should prompt the height and the width of the car].

*/



/* The car */

 void draw_car(Position car){
	mvprintw(car.y, car.x, "  -- ");
	mvprintw(car.y + 1, car.x + 1, "|__|");
}

/*The obstacles */
 
 void draw_obstacles(Position obstacle){
	mvprintw(obstacle.y, obstacle.x, "[**]");
	mvprintw(obstacle.y + 1, obstacle.x + 1, "[**]");
}

 void update_obstacles(Position *obstacles, int num_obstacles, int speed){
 
 int i;
 for(i=0; i < num_obstacles; i++){
        obstacles[i].y += speed;
	// out of bound checking
	if(obstacles[i].y > ROAD_HEIGHT){
		obstacles[i].y = 0;
		obstacles[i].x = rand() % (COLS - OBSTACLE_WIDTH);
	}
	draw_obstacles(obstacles[i]);
   }
 
 }


/* collision checking : car vs obstacles */

int check_collision(Position car, Position *obstacles, int num_obstacles){
    
 int i;
 for(i = 0; i <  num_obstacles; i++){
  
 if(car.y < obstacles[i].y + OBSTACLE_HEIGHT
	 && car.y + CAR_HEIGHT > obstacles[i].y 
	 && car.x < obstacles[i].x + OBSTACLE_WIDTH
	 && car.x + CAR_WIDTH > obstacles[i].x){
		return 1; // collision detected
	}



 }
 return 0;


}
 /* Increase speed */

 void increase_speed(int *speed, int score){
	if(score % 10 == 0 && *speed < 3)
		(*speed) ++;
}



 int main(){
  int i;

  initscr();
  
  srand(getpid());

  keypad(stdscr, TRUE);
  noecho();

  curs_set(0);

  nodelay(stdscr, TRUE);

  Position car = {COLS / 2, ROAD_HEIGHT - 3};
  
  int speed = 1;
  int score = 0;

  int num_obstacles = 3;
  
  Position obstacles[3];


  /* Initialize the obstacle */ 
 	

 for(i=0; i <  num_obstacles; i++){
	obstacles[i].x = rand() % (COLS - OBSTACLE_WIDTH);
	obstacles[i].y = rand() % ROAD_HEIGHT;
 }

 while(1){
 clear();

 mvprintw(0, 3, "Score %d", score);
 mvprintw(1, 3, "Press q to quit");
 /* Display the road */

 for(i=0; i <  ROAD_HEIGHT;i++){
	mvprintw(i, 0, "|");
	mvprintw(1, COLS - 1, "|");
 } 
 
/* Draw the car */

 draw_car(car);
 
 /* Update the obstacles */

 update_obstacles(obstacles, num_obstacles, speed);
 
 if(check_collision(car, obstacles, num_obstacles)){
	mvprintw(ROAD_HEIGHT / 2, COLS / 2 - 5, "GAME OVER!");
	refresh();
	sleep(3);

	break;
 }

 /* The user should press q to quit */
 
 int ch = getch();
 
 if(ch == 'q') break;
 
 if(ch == KEY_LEFT && car.x > 1) car.x --;
 if(ch == KEY_RIGHT && car.x < COLS - CAR_WIDTH - 1) car.x ++;

 score++;

 increase_speed(&speed, score);
 
 refresh();

 usleep(100000 / speed);

}

 endwin();


 return 0;
}

