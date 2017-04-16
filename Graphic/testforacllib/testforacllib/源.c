#include "acllib.h"
#include <math.h>

#define new(TYPENAME) ((TYPENAME*)malloc(sizeof(TYPENAME)))

typedef double data_type;

struct Vector2
{
	data_type x;
	data_type y;
};

typedef struct Vector2 Vector2;
typedef Vector2 Point2;

#define TWO_DEMINSION

#ifdef TWO_DEMINSION
typedef Vector2 Vector;
typedef Point2 Point;
#endif


struct point
{
	Point position;
	Vector velocity;
};

typedef struct point point;
typedef point* pointer;

pointer player;

#define KEY_W 0X57
#define KEY_A 0X41
#define KEY_D 0X44
#define KEY_S 0X53

void set_initial_position(){}

void player_control(int key, ACL_Keyboard_Event event)
{
	
}
/*
int Setup()
{


	return 0;
}*/