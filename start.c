/*
Miniature Autonomous Vehicle Code
Authors: Chris Savoca, Steven James, Chris Michel, Shamita Nagalla
EECE 4376
*/

#include "../../libraries/rc_usefulincludes.h"
#include "../../libraries/roboticscape.h"


double leftDuty = 0.0;
double rightDuty = 0.0;
static int true = 1;
static int false = 0;
static int leftPin = 57;
static int rightPin = 49;

void findLineLeft();
void findLineRight();
int getRightPin();
int getLeftPin();


/* 
Motor functions
Chris Savoca
*/

void stop()
{
	leftDuty = 0.0;
	rightDuty = 0.0;
	printf("Stopping\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void moveForward() 
{
	leftDuty = 0.14;
	rightDuty = 0.14;
	printf("Moving forward\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void moveBackward() 
{
	leftDuty = -0.14;
	rightDuty = -0.14;
	//printf("Moving backward\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void rotateLeft() 
{
	leftDuty = -0.14;
	rightDuty = 0.14;
	printf("Rotating left\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void rotateRight() 
{
	leftDuty = 0.14;
	rightDuty = -0.14;
	printf("Rotating right\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void adjustLeft() 
{
	rightDuty += 0.08;
	leftDuty -= 0.08;
	printf("Adjusting left\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void adjustRight() 
{
	leftDuty += 0.08;
	rightDuty -= 0.08;
	printf("Adjusting right\n");
	rc_set_motor(3, leftDuty);
	rc_set_motor(4, rightDuty);
}

void turnLeft()
{
	printf("Turning left\n");
	rotateLeft();
	usleep(1000000);
	findLineLeft();
}

void turnRight()
{
	printf("Turning right\n");
	rotateRight();
	usleep(1000000);
	findLineRight();
}

void turnAround() 
{
	printf("Turning around\n");
	rotateRight();
	usleep(2000000);
	findLineRight();
}

void findLineRight()
{
	int notOnLine = true;
	if (getLeftPin() == 1) {
		turnRight();
	} else {
	adjustRight();
	while (notOnLine) 
	{
		if (getRightPin() == 0) 
		{
			moveForward();
			notOnLine = false;
		}
	}
	}
	
}

void findLineLeft() 
{
	int notOnLine = true;
	if(getRightPin() == 1) {
		turnRight();
	} else {
		adjustLeft();
		while (notOnLine) 
		{
			if (getLeftPin() == 0) 
			{
				moveForward();
				notOnLine = false;
			}
		}
	}

}

// end motor functions

int getRightPin()
{
	//printf("%d\n", rc_gpio_get_value(rightPin));
	return rc_gpio_get_value(rightPin);
}

int getLeftPin()
{
	//printf("%d\n", rc_gpio_get_value(leftPin));
	return rc_gpio_get_value(leftPin);
}

// sets up GPIO pins
int setup(){
	if(rc_initialize()){
		fprintf(stderr,"ERROR: failed to run rc_initialize(), are you root?\n");
		return -1;
	}

	if(rc_gpio_export(leftPin)){
		fprintf(stderr,"ERROR: Failed to export gpio pin %d\n", leftPin);
		return -1;
	}
	if(rc_gpio_set_dir(leftPin, INPUT_PIN)){
		fprintf(stderr,"ERROR: Failed to set gpio pin %d as output\n", leftPin);
		return -1;
	}
	if(rc_gpio_export(rightPin)){
		fprintf(stderr,"ERROR: Failed to export gpio pin %d\n", rightPin);
		return -1;
	}
	if(rc_gpio_set_dir(rightPin, INPUT_PIN)){
		fprintf(stderr,"ERROR: Failed to set gpio pin %d as output\n", rightPin);
		return -1;
	}
	return 0;
}

void x_inc(char* orient)
{
	printf("Increment x\n");
	if (*orient == 'n')
	{
		turnRight();
		*orient = 'e';
		moveForward();
	}
	if (*orient == 's')
	{
		turnLeft();
		*orient = 'e';
		moveForward();
	}
	if (*orient == 'e')
	{
		moveForward();
		usleep(100000);
	}
	if (*orient == 'w')
	{
		turnAround();
		*orient = 'e';
		moveForward();
	}
}

void y_inc(char* orient)
{
	printf("Increment y\n");
	if (*orient == 'n')
	{
		moveForward();
		usleep(100000);
	}
	if (*orient == 's')
	{
		turnAround();
		*orient = 'n';
		moveForward();
	}
	if (*orient == 'e')
	{
		turnLeft();
		*orient = 'n';
		moveForward();
	}
	if (*orient == 'w')
	{
		turnRight();
		*orient = 'n';
		moveForward();
	}
}

void x_dec(char* orient)
{
	printf("Decrement x\n");
	if (*orient == 'n')
	{
		turnLeft();
		*orient = 'w';
		moveForward();
	}
	if (*orient == 's')
	{
		turnRight();
		*orient = 'w';
		moveForward();
	}
	if (*orient == 'e')
	{
		turnAround();
		*orient = 'w';
		moveForward();
	}
	if (*orient == 'w')
	{
		moveForward();
		usleep(100000);
	}
}

void y_dec(char* orient)
{
	printf("Decrement y\n");
	if (*orient == 'n')
	{
		turnAround();
		*orient = 's';
		moveForward();
	}
	if (*orient == 's')
	{
		moveForward();
		usleep(100000);
	}
	if (*orient == 'e')
	{
		turnRight();
		*orient = 's';
		moveForward();
	}
	if (*orient == 'w')
	{
		turnLeft();
		*orient = 's';
		moveForward();
	}
}

char getColor() 
{
	uint8_t buf[5] = {0}; 
	uint8_t red;
	uint8_t green;
	uint8_t blue;
	int ret;
	ret = rc_i2c_read_bytes(1, 0x16|0x80, 2, buf); 
	red = buf[1];
	ret = rc_i2c_read_bytes(1, 0x18|0x80, 2, buf); 
	green = buf[1];
	ret = rc_i2c_read_bytes(1, 0x1a|0x80, 2, buf); 
	blue = buf[1];
	
	
	//identify ranges
	//red (1)
	if (red <= 92 && red >= 88 && green <= 32 && green >= 28 && blue <= 28 && blue >= 24) {
		printf("Found red\n");
		return 'r';
	// light green (2)
	} else if (red <= 38 && red >= 34 && green <= 65 && green >= 61 && blue <= 34 && blue >= 30) {
		printf("Found light green\n");
		return 'l';
	// blue (3)
	} else if (red <= 15 && red >= 11 && green <= 25 && green >= 21 && blue <= 41 && blue >= 37) {
		printf("Found blue\n");
		return 'b';
	// orange (4)
	} else if (red <= 97 && red >= 93 && green <= 44 && green >= 40 && blue <= 31 && blue >= 27) {
		printf("Found orange\n");
		return 'o';
	// yellow (5)
	} else if (red <= 133 && red >= 129 && green <= 129 && green >= 125 && blue <= 52 && blue >= 48) {
		printf("Found yellow\n");
		return 'y';
	// pink (6)
	} else if (red <= 81 && red >= 77 && green <= 25 && green >= 21 && blue <= 39 && blue >= 35) {
		printf("Found pink\n");
		return 'p';
	// purple (7)
	} else if (red <= 24 && red >= 18 && green <= 22 && green >= 18 && blue <= 37 && blue >= 33) {
		printf("Found purple\n");
		return 'u';
	// cyan (8)
	} else if (red <= 26 && red >= 22 && green <= 66 && green >= 62 && blue <= 78 && blue <= 74) {
		printf("Found cyan\n");
		return 'c';
	// dark green (9)
	} else if (red <= 28 && red >= 24 && green <= 53 && green >= 49 && blue <= 34 && blue >= 30) {
		printf("Found green\n");
		return 'g';
	// other (black, white)
	} else {
		//printf("Returning 'x'\n");
		return 'x';
	}
	
}

struct Node 
{
	int x;
	int y;
	int north;
	int south;
	int east;
	int west;
};

static struct Node red;
static struct Node orange;
static struct Node purple;
static struct Node lightGreen;
static struct Node yellow;
static struct Node green;
static struct Node cyan;
static struct Node pink;
static struct Node blue;

void initializeCoordinates()
{
	red.x = 3;
	red.y = 1;
	red.north = 1;
	red.south = 0;
	red.east = 0;
	red.west = 1;
	
	orange.x = 2;
	orange.y = 1;
	orange.north = 1;
	orange.south = 0;
	orange.east = 1;
	orange.west = 1;
	
	purple.x = 1;
	purple.y = 1;
	purple.north = 1;
	purple.south = 0;
	purple.east = 1;
	purple.west = 0;
	
	lightGreen.x = 3;
	lightGreen.y = 2;
	lightGreen.north = 1;
	lightGreen.south = 1;
	lightGreen.east = 0;
	lightGreen.west = 1;
	
	yellow.x = 2;
	yellow.y = 2;
	yellow.north = 1;
	yellow.south = 1;
	yellow.east = 1;
	yellow.west = 1;
	
	green.x = 1;
	green.y =2;
	green.north = 1;
	green.south = 1;
	green.east = 1;
	green.west = 0;
	
	cyan.x = 1;
	cyan.y = 3;
	cyan.north = 0;
	cyan.east = 1;
	cyan.south = 1;
	cyan.east = 0;
	
	pink.x = 2;
	pink.y = 3;
	pink.north = 0;
	pink.east = 1;
	pink.south = 1;
	pink.east = 1;
	
	blue.x = 3;
	blue.y = 3;
	blue.north = 0;
	blue.east = 0;
	blue.south = 0;
	blue.west = 0;
}

struct Node* getNode(char color) 
{
	if (color == 'r') {
		printf("Return red\n");
		return &red;
	} else if (color == 'l') {
		return &lightGreen;
	} else if (color == 'b') {
		return &blue;
	} else if (color == 'o') {
		return &orange;
	} else if (color == 'y') {
		return &yellow;
	} else if (color == 'p') {
		return &pink;
	} else if (color == 'u') {
		return &purple;
	} else if (color == 'c') {
		return &cyan;
	} else if (color == 'g') {
		return &green;
	} 
	return NULL;
}

// checks for obstacle and updates direction pointers
void checkForObstacle(struct Node* node, char* orient, int analog_in){
	//printf("We are checking for obstacles\n");
	float value = rc_adc_volt(analog_in); // read in from adc
	if(value > 1.25){ // there is an obstacle
	
		// remove this leg from the map
		if(*orient == 'n'){
			node->north = 0;
			*orient = 's';
		} 
		if(*orient == 'e'){
			node->east = 0;
			*orient = 'w';
		} 
		if(*orient == 's') {
			node->south = 0;
			*orient = 'n';
		}
		
		if(*orient == 'w'){
			node->west = 0;
			*orient = 'e';
		} 
		
	    // TURN AROUND
		turnAround(); 
		
	}
	//printf("adc_%d\t %6.2f\n", analog_in, value);
	fflush(stdout);
	//rc_usleep(100000);
}

int main(){


	// initialize hardware first
	setup();
	
	// bring H-bridges of of standby
	rc_enable_motors(); 
	
	//enable color sensor
	int ret = rc_i2c_init(1, 0x29); 
	
	// update intersection coordinates
	initializeCoordinates();

	// find inital line to follow
	rotateRight();
	int lineNotFound = true;
	while(lineNotFound)
	{
		if (getRightPin() == 1) {
			printf("Finding line\n");
			findLineRight();
			stop();
			lineNotFound = false; ;
		}
		
	}
	
	printf("Line found\n");
	
	// get dest
	char destChar;
	printf("Cyan = 'c'\nPink = 'p'\nBlue = 'b'\nDark Green = 'g'\nYellow = 'y'\nLight Green = 'l'\nPurple = 'p'\nOrange = 'o'\nRed = 'r'\n");
	printf("Enter destination: \n");
	
	do {
		destChar = getchar();
	} while (isspace(destChar));
	
	printf("Destination received: %c\n", destChar);
	struct Node *dest = getNode(destChar);
	fflush(stdin);
	fflush(stdout);
	char orient;
	printf("Which direction is the robot facing? (n,e,s,w) ");
	fflush(stdout);
	do {
		orient = getchar();
	} while (isspace(orient));	
	
	usleep(1000000);
	
	moveForward();
	
	int notAtDest = true;

	// begin line following and intersection detection
	int analog_in = 3;
	while (notAtDest && rc_get_state()!=EXITING)
	{
		
		if (getRightPin() == 1)	{
			findLineRight();
		}
		
		if (getLeftPin() == 1) {
			findLineLeft();
		}
		
		struct Node* cur;
		
		char color = getColor();
		if (color != 'x') {
			cur = getNode(color);
			if (cur->x < dest->x && cur->east != 0) {
				x_inc(&orient);
			} else if (cur->x > dest->x && cur->west != 0) {
				x_dec(&orient);
			} else if (cur->y < dest->y && cur->north != 0) {
				y_inc(&orient);
			} else if (cur->y > dest->y && cur->south != 0) {
				y_dec(&orient);
			} else if (cur->east!= 0) {
				cur->east = 0;
				x_inc(&orient);
			} else if (cur->west != 0) {
				cur->west = 0;
				x_dec(&orient);
			} else if (cur->north != 0) {
				cur->north = 0;
				y_inc(&orient);
			} else if (cur->south != 0) {
				cur->south = 0;
				y_dec(&orient);
			} else if (cur->x == dest->x && cur->y == dest->y) {
				notAtDest = false;
			} else {
				//panic();
			}
		
		}
		
		// constantly poll for obstacles
		checkForObstacle(cur, &orient, analog_in);
		
	}

	// final cleanup
	rc_cleanup();
	
	return 0;
}
