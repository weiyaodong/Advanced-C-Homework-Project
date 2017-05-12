#include "acllib.h"
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define new(TYPENAME) (TYPENAME*)malloc(sizeof (TYPENAME))

#define begin beginPaint()
#define end endPaint()

#define MAX_PAINTING_AREA_WIDTH 840
#define MAX_PAINTING_AREA_HEIGHT 640

typedef enum
{
	LINE,
	RECTANGLE,
	ELLIPSE,
	POLYGON,
	PENCIL,
	ERASER,
	EJECTION_GUN
} Painter_Mode;

typedef enum
{
	PAINTING,
	STOP
} Painting_Mode;

typedef enum {
	false,
	true
} bool;

typedef enum
{
	RELEASED,
	PRESSED
} Button_State;

typedef enum
{
	TOTAL_CONTAINER,
	TOOL_BOX,
	PAINTING_AREA,
	TOOL_PENCIL,
	TOOL_LINE,
	TOOL_RECTANGLE,
	TOOL_ELLIPSE,
	TOOL_ERASER,
	TOOL_EJECTION_GUN,
	TOOL_POLYGON,
	COLOR_BOX,
	COLOR_UNIT,
	COLOR_VIEW,
	BUTTON_CANCEL,
	BUTTON_REDO,
	BUTTON_QUIT
} Widget_Type;

struct WidgetList;

typedef struct WidgetList* node_pointer;

struct Widget
{
	int left;
	int top;
	int right;
	int bottom;

	ACL_Color color;

	struct Widget* children[12];
	int child_num;
	
	Widget_Type type;
};

struct Shape
{
	int left;
	int top;
	int right;
	int bottom;

	Painter_Mode type;
	ACL_Color color;

	struct Shape* next;

};

typedef struct Shape* list_node;

list_node head_node;
int total_shape;

void add_shape(list_node insert_node)
{
	total_shape++;
	if(head_node == NULL)
	{
		head_node = insert_node;
		return;
	}
	list_node current;
	// ReSharper disable once CppPossiblyErroneousEmptyStatements
	for (current = head_node; current->next != NULL; current = current->next);
	current->next = insert_node;
}

void pop_shape()
{
	total_shape--;
	if(total_shape == 0)
	{
		free(head_node);
		head_node = NULL;
		return;
	}
	list_node current = head_node;
	for(int i = 0; i < total_shape - 1; i++)
	{
		current = current->next;
	}
	free(current->next);
	current->next = NULL;
}

typedef struct Widget Widget;

typedef Widget* widget_pointer;

// global variables here

ACL_Color front_color = BLACK;
ACL_Color back_color = EMPTY;

Widget main_window;
Widget paint_area;
Widget tool_box;
Widget pencil_button;
Widget eraser_button;
Widget rectangle_button;
Widget ellipse_button;
Widget line_button;
Widget ejec_gun_button;
Widget polygon_button;
Widget cancel_button;
Widget redo_button;

Widget color_view_box;
Widget color_box;
Widget color_unit_box[12];


int last_mouse_x = -1;
int last_mouse_y = -1;
int last_mouse_x2 = -1;
int last_mouse_y2 = -1;

Painter_Mode painter_mode = LINE;
Painting_Mode painting_mode = STOP;
Button_State left_button_state = RELEASED;
Button_State right_button_state = RELEASED;
Button_State key_state[0xff];

Painter_Mode temp_painter_mode;


void paint_widget(widget_pointer current_widget)
{
	if (current_widget == NULL) return;
	switch (current_widget->type)
	{
	case TOTAL_CONTAINER:
		break;
	case TOOL_BOX:
		begin; 
		setPenColor(LIGHT_GREY);
		setBrushColor(LIGHT_GREY);
		rectangle(0, 0, 120, 640);
		end;
		break;
	case PAINTING_AREA:
		break;
	case TOOL_POLYGON:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		rectangle(20, 310, 100, 340);
		setTextSize(20);
		setTextColor(DARK_GREY);
		paintText(25, 315, " TO DO ");
		end;
		break;
	case TOOL_LINE:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		setTextSize(20);
		rectangle(20, 210, 100, 240);
		setTextColor(DARK_GREY);
		paintText(40, 215, "LINE");
		end;
		break;
	case TOOL_PENCIL:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		setTextSize(20);
		rectangle(20, 10, 100, 40);
		setTextColor(DARK_GREY);
		paintText(30, 15, " TODO ");
		end;

		break;
	case TOOL_EJECTION_GUN:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		setTextSize(20);
		rectangle(20, 260, 100, 290);
		setTextColor(DARK_GREY);
		paintText(25, 265, " TO DO ");
		end;
		break;
	case TOOL_ELLIPSE:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		setTextSize(20);
		rectangle(20, 160, 100, 190);
		setTextColor(DARK_GREY);
		paintText(25, 165, "ELLIPSE");
		end;
		break;
	case TOOL_ERASER:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		setTextSize(20);
		rectangle(20, 60, 100, 90);
		setTextColor(DARK_GREY);
		paintText(30, 65, " TODO ");
		end;
		break;
	case TOOL_RECTANGLE:
		begin;
		setPenColor(GREY);
		setBrushColor(GREY);
		setTextBkColor(GREY);
		setTextSize(20);
		rectangle(20, 110, 100, 140);
		setTextColor(DARK_GREY);
		paintText(40, 115, "RECT");
		end;
		break;
	case BUTTON_CANCEL:
		begin;
		setPenColor(LIGHT_RED);
		setTextSize(20);
		setBrushColor(LIGHT_RED);
		setTextBkColor(LIGHT_RED);
		rectangle(20, 360, 100, 390);
		setTextColor(WHITE);
		paintText(30, 365, "CANCEL");
		end;
		break;
	case BUTTON_REDO:
		begin;
		setPenColor(GREEN);
		setBrushColor(GREEN);
		setTextBkColor(GREEN);
		rectangle(20, 410, 100, 440);
		setTextSize(20);
		setTextColor(WHITE);
		paintText(40, 415, "TODO");
		end;
		break;
	case COLOR_BOX:
		break;
	case COLOR_UNIT:
		begin;
		setPenColor(current_widget->color);
		setBrushColor(current_widget->color);
		rectangle(current_widget->left, current_widget->top, current_widget->right, current_widget->bottom);
		end;
		break;
	case COLOR_VIEW:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(front_color);		
		rectangle(20, 460, 60, 480);
		setBrushColor(back_color);
		rectangle(60, 460, 100, 480);
		end;
		break;
	default:
		assert(0);
	}
}

void paint_shape(list_node current)
{
	begin;
	switch(current->type)
	{
	case RECTANGLE:
		setPenColor(current->color);
		setBrushColor(EMPTY);
		rectangle(current->left, current->top, current->right, current->bottom);
		break;
	case ELLIPSE:
		setPenColor(current->color);
		setBrushColor(EMPTY);
		ellipse(current->left, current->top, current->right, current->bottom);
		break;
	case LINE:
		setPenColor(current->color);
		setBrushColor(EMPTY);
		line(current->left, current->top, current->right, current->bottom);
		break;
	default:
		break;
	}
	end;
}

void paint_all_shapes()
{
	for (list_node current = head_node; current != NULL; current = current->next)
	{
		paint_shape(current);
	}
}

void set_widget_pos(Widget* current, int left,  int top, int right, int bottom)
{
	current->left = left;
	current->top = top;
	current->right = right;
	current->bottom = bottom;
}

void init()
{
	set_widget_pos(&main_window, 0, 0, 960, 640);
	main_window.type = TOTAL_CONTAINER;
	main_window.children[0] = &tool_box;
	main_window.children[1] = &paint_area;
	main_window.child_num = 2;

	set_widget_pos(&paint_area, 120, 0, 960, 640);
	paint_area.type = PAINTING_AREA;
	paint_area.child_num = 0;

	set_widget_pos(&tool_box, 0, 0, 120, 640);
	tool_box.type = TOOL_BOX;
	tool_box.children[0] = &pencil_button;
	tool_box.children[1] = &eraser_button;
	tool_box.children[2] = &rectangle_button;
	tool_box.children[3] = &ellipse_button;
	tool_box.children[4] = &line_button;
	tool_box.children[5] = &ejec_gun_button;
	tool_box.children[6] = &polygon_button;
	tool_box.children[7] = &cancel_button;
	tool_box.children[8] = &redo_button;
	tool_box.children[9] = &color_view_box;
	tool_box.children[10] = &color_box;
	tool_box.child_num = 11;

	set_widget_pos(&pencil_button, 20, 10, 100, 40);
	pencil_button.type = TOOL_PENCIL;
	pencil_button.child_num = 0;

	set_widget_pos(&eraser_button, 20, 60, 100, 90);
	eraser_button.type = TOOL_ERASER;
	eraser_button.child_num = 0;

	set_widget_pos(&rectangle_button, 20, 110, 100, 140);
	rectangle_button.type = TOOL_RECTANGLE;
	rectangle_button.child_num = 0;

	set_widget_pos(&ellipse_button, 20, 160, 100, 190);
	ellipse_button.type = TOOL_ELLIPSE;
	ellipse_button.child_num = 0;

	set_widget_pos(&line_button, 20, 210, 100, 240);
	line_button.type = TOOL_LINE;
	line_button.child_num = 0;

	set_widget_pos(&ejec_gun_button, 20, 260, 100, 290);
	ejec_gun_button.type = TOOL_EJECTION_GUN;
	ejec_gun_button.child_num = 0;

	set_widget_pos(&polygon_button, 20, 310, 100, 340);
	polygon_button.type = TOOL_POLYGON;
	polygon_button.child_num = 0;

	set_widget_pos(&cancel_button, 20, 360, 100, 390);
	cancel_button.type = BUTTON_CANCEL;
	cancel_button.child_num = 0;

	set_widget_pos(&redo_button, 20, 410, 100, 440);
	redo_button.type = BUTTON_REDO;
	redo_button.child_num = 0;

	set_widget_pos(&color_view_box, 20, 460, 100, 480);
	color_view_box.type = COLOR_VIEW;
	color_view_box.child_num = 0;

	set_widget_pos(&color_box, 20, 500, 100, 620);
	color_box.type = COLOR_BOX;
	color_box.child_num = 12;

	for(int i = 0; i < 12; i++)
	{
		color_box.children[i] = &color_unit_box[i];
		int x = (i) / 2;
		int y = (i) % 2;
		set_widget_pos(&color_unit_box[i], 20 + y * 40, 500 + 20 * x, 60 + y * 40, 520 + 20 * x);
		color_unit_box[i].type = COLOR_UNIT;
		color_unit_box[i].child_num = 0;

	}


	color_unit_box[0].color = BLACK;
	color_unit_box[1].color = EMPTY;
	color_unit_box[2].color = GREEN;
	color_unit_box[3].color = BLUE;
	color_unit_box[4].color = CYAN;
	color_unit_box[5].color = MAGENTA;
	color_unit_box[6].color = YELLOW;
	color_unit_box[7].color = RED;
	color_unit_box[8].color = GREY;
	color_unit_box[9].color = LIGHT_RED;
	color_unit_box[10].color = LIGHT_GREEN;
	color_unit_box[11].color = LIGHT_PURPLE;


	
}

void paint_box(Widget* current)
{
	paint_widget(current);
	for (size_t i = 0; i < current->child_num; i++)
	{
		paint_box(current->children[i]);
	}
}

void paint_highlight()
{
	switch (painter_mode)
	{
	case POLYGON:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		rectangle(20, 310, 100, 340);
		setTextSize(20);
		setTextColor(WHITE);
		paintText(25, 315, " TO DO ");
		end;
		break;
	case LINE:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		setTextSize(20);
		rectangle(20, 210, 100, 240);
		setTextColor(WHITE);
		paintText(40, 215, "LINE");
		end;
		break;
	case PENCIL:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		setTextSize(20);
		rectangle(20, 10, 100, 40);
		setTextColor(WHITE);
		paintText(30, 15, " TODO ");
		end;
		break;
	case EJECTION_GUN:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		setTextSize(20);
		rectangle(20, 260, 100, 290);
		setTextColor(WHITE);
		paintText(25, 265, " TO DO ");
		end;
		break;
	case ELLIPSE:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		setTextSize(20);
		rectangle(20, 160, 100, 190);
		setTextColor(WHITE);
		paintText(25, 165, "ELLIPSE");
		end;
		break;
	case ERASER:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		setTextSize(20);
		rectangle(20, 60, 100, 90);
		setTextColor(WHITE);
		paintText(30, 65, " TODO ");
		end;
		break;
	case RECTANGLE:
		begin;
		setPenColor(DARK_GREY);
		setBrushColor(DARK_GREY);
		setTextBkColor(DARK_GREY);
		setTextSize(20);
		rectangle(20, 110, 100, 140);
		setTextColor(WHITE);
		paintText(40, 115, "RECT");
		end;
		break;
	default:
		assert(0);
	}
}

int check_in_widget(Widget* current, int x, int y)
{
	if (x < current->left || x >= current->right || y < current->top || y >= current->bottom)
		return 0;
	return 1;
}

Widget* find_widget(Widget* current, int x, int y)
{
	if (!check_in_widget(current, x, y))	return NULL;
	for (int i = 0; i < current->child_num; i++)
	{
		if (check_in_widget(current->children[i], x, y))
		{
			return find_widget(current->children[i], x, y);
		}
	}
	return current;
}

void keyboard_event_callback_function(int key_code, ACL_Keyboard_Event event)
{
//	if (event == KEY_DOWN) key_state[key_code] = PRESSED;
//	if (event == KEY_UP) key_state[key_code] = RELEASED;
//	switch (key_code)
//	{
//	case 0x45:
//		if(event == KEY_DOWN)
//		{
//			if(painter_mode != ERASER)
//			{
//				temp_painter_mode = painter_mode;
//				painter_mode = ERASER;
//			}
//		}
//		else if(event == KEY_UP)
//		{
//			if(temp_painter_mode != ERASER)
//			{
//				painter_mode = temp_painter_mode;
//			}
//		}
//		break;
//	default:
//		;
//	}
//	paint_box(&main_window);
//	paint_highlight();
}

POINT temp_for_poly_Bezier[4];
int Bezier_counter;

int painting_area_back_up[MAX_PAINTING_AREA_WIDTH][MAX_PAINTING_AREA_HEIGHT];
bool modified = false;

int start_pos_x;
int start_pos_y;

int move_counter;

void make_painting_area_back_up()
{
	begin;
	int current_time = time(0);
	for(int i = 120; i < 960; i++)
	{
		for(int j = 0; j < 640; j++)
		{
			painting_area_back_up[i - 120][j] = getPixel(i, j);
		}
	}
	printf("time = %lld\n", time(0) - current_time);
	modified = true;
	end;
}

void painting_area_recover()
{
	begin;
	for (int i = 120; i < 960; i++)
	{
		for (int j = 0; j < 640; j++)
		{
			putPixel(i, j, painting_area_back_up[i - 120][j]);
		}
	}
	end;
}

void mouse_event_callback_function(int x, int y, ACL_Mouse_Button button, ACL_Mouse_Event event)
{
	Widget* current_block = find_widget(&main_window, x, y);
	if (current_block == NULL) return;
	
	if(event == BUTTON_DOWN)
	{
		if (button == LEFT_BUTTON) left_button_state = PRESSED;
		if (button == RIGHT_BUTTON) right_button_state = PRESSED;
	}
	else if (event == BUTTON_UP)
	{
		if (button == LEFT_BUTTON) left_button_state = RELEASED;
		if (button == RIGHT_BUTTON) right_button_state = RELEASED;
	}
	printf("(%d, %d) painting_mode: %s\n", x, y, painting_mode ? "STOP" : "PAINTING");
	switch (current_block->type)
	{
	case PAINTING_AREA:
		if(painter_mode == PENCIL)
		{
			// TO DO
		}
		else if((painter_mode == ERASER))
		{
			// TO DO
		}
		else if(painter_mode == ELLIPSE || painter_mode == RECTANGLE)
		{
			if (button == LEFT_BUTTON && painting_mode == STOP && event == BUTTON_DOWN)
			{
				painting_mode = PAINTING;
				start_pos_x = x;
				start_pos_y = y;
				list_node new_node = new(struct Shape);
				new_node->next = NULL;
				int min_x = start_pos_x;
				int max_x = x;
				if (start_pos_x > x)
				{
					min_x = x;
					max_x = start_pos_x;
				}
				int min_y = start_pos_y;
				int max_y = y;
				if (start_pos_y > y)
				{
					min_y = y;
					max_y = start_pos_y;
				}
				new_node->top = min_y;
				new_node->left = min_x;
				new_node->right = max_x;
				new_node->bottom = max_y;
				new_node->color = front_color;
				new_node->type = painter_mode;

				add_shape(new_node);
			}
			else if (painting_mode == PAINTING)
			{
				if (event == BUTTON_DOWN && button == LEFT_BUTTON)
				{
					painting_mode = STOP;
				}
				else if (event == MOUSEMOVE)
				{
					pop_shape();
					list_node new_node = new(struct Shape);
					new_node->next = NULL;
					int min_x = start_pos_x;
					int max_x = x;
					if (start_pos_x > x)
					{
						min_x = x;
						max_x = start_pos_x;
					}
					int min_y = start_pos_y;
					int max_y = y;
					if (start_pos_y > y)
					{
						min_y = y;
						max_y = start_pos_y;
					}
					new_node->top = min_y;
					new_node->left = min_x;
					new_node->right = max_x;
					new_node->bottom = max_y;
					new_node->color = front_color;
					new_node->type = painter_mode;

					add_shape(new_node);
				}
			}
		}
		else if (painter_mode == LINE)	
		{
			if(button == LEFT_BUTTON && painting_mode == STOP && event == BUTTON_DOWN)
			{
				painting_mode = PAINTING;
				start_pos_x = x;
				start_pos_y = y;
				list_node new_node = new(struct Shape);
				new_node->next = NULL;
				new_node->top = start_pos_y;
				new_node->left = start_pos_x;
				new_node->right = x;
				new_node->bottom = y;
				new_node->color = front_color;
				new_node->type = painter_mode;

				add_shape(new_node);
			}
			else if(painting_mode == PAINTING)
			{
				if(event == BUTTON_DOWN && button == LEFT_BUTTON)
				{
					painting_mode = STOP;
				}
				else if(event == MOUSEMOVE)
				{
					pop_shape();
					list_node new_node = new(struct Shape);
					new_node->next = NULL;
					new_node->top = start_pos_y;
					new_node->left = start_pos_x;
					new_node->right = x;
					new_node->bottom = y;
					new_node->color = front_color;
					new_node->type = painter_mode;

					add_shape(new_node);
				}
			}
		}
		break;
	case COLOR_UNIT:
		if (event == BUTTON_DOWN)
		{
			if(button == LEFT_BUTTON)
			{
				front_color = current_block->color;
			}
			else if(button == RIGHT_BUTTON)
			{
				back_color = current_block->color;
			}
			painting_mode = STOP;
		}
		break;
	case TOOL_PENCIL:
		if (event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = PENCIL;
			painting_mode = STOP;
		}
		break;
	case TOOL_ERASER:
		if(event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = ERASER;
			painting_mode = STOP;
		}
		break;
	case TOOL_RECTANGLE:
		if(event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = RECTANGLE;
			painting_mode = STOP;
		}
		break;
	case TOOL_ELLIPSE:
		if (event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = ELLIPSE;
			painting_mode = STOP;
		}
		break;
	case TOOL_LINE:
		if (event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = LINE;
			painting_mode = STOP;
		}
		break;
	case TOOL_EJECTION_GUN:
		if (event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = EJECTION_GUN;
			painting_mode = STOP;
		}
		break;
	case TOOL_POLYGON:
		if (event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			painter_mode = POLYGON;
			painting_mode = STOP;
		}
		break;
	case BUTTON_CANCEL:
		if (event == BUTTON_DOWN && button == LEFT_BUTTON)
		{
			//painting_area_recover();
			//modified = false;
			pop_shape();
			painting_mode = STOP;
		}
		break;
	case BUTTON_REDO:
		// TO DO
		break;
		
	default:
		break;
	}
	begin;
	clearDevice();
	end;
	paint_box(&main_window);
	paint_highlight();
	paint_all_shapes();

	if(current_block->type != TOTAL_CONTAINER 
		&& current_block->type != PAINTING_AREA 
		&& current_block->type != TOOL_BOX
		&& current_block->type != COLOR_VIEW
		&& current_block->type != COLOR_BOX)
	{
		begin;
		for(int i = current_block->left; i < current_block->right; i++)
		{
			for(int j = current_block->top; j < current_block->bottom; j++)
			{
				int temp_rgb = RGB(((getPixel(i, j) & 0xff) + 0xff)>>1 , ((getPixel(i, j) & 0xff00) + 0xff00) >> 9,((getPixel(i, j) & 0xff0000) + 0xff0000) >> 17);
				putPixel(i, j, temp_rgb);
			}
		}
		end;
	}

	last_mouse_x2 = last_mouse_x;
	last_mouse_y2 = last_mouse_y;
	last_mouse_x = x;
	last_mouse_y = y;
}

int Setup()
{
	initWindow("painting", DEFAULT, DEFAULT, 960, 640);
	initConsole();
	init();
	paint_box(&main_window);
	registerMouseEvent(mouse_event_callback_function);
	registerKeyboardEvent(keyboard_event_callback_function);

	return 1;
}
