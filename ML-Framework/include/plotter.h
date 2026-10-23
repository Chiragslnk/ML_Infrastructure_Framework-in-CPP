#pragma once
#include<vector>
#include<iostream>
using namespace std;
typedef struct _shape
{
char type;
}shape;
typedef struct _shape_point
{
char type;
double x,y;
}shape_point;
typedef struct _shape_line
{
char type;
shape_point *point_1,*point_2;
}shape_line;
shape_point * point(double x,double y); 
shape_line * line(shape_point *point_1,shape_point *point_2);
class plotter
{
private:
vector<shape *> shapes;
pair<int,int> x_range;
pair<int,int> y_range;
pair<int,int> size;
pair<int,int> location;
string x_label;
string y_label;
FILE *gnuplot;
string title;
int terminal_number;
public:
plotter();
void add(shape_point *point);
void add(shape_line *line);
void remove(char type);
void clear();
void plot();
void set_x_range(pair<int,int> x_range);
void set_y_range(pair<int,int> y_range);
void set_title(string title);
void set_size(pair<int,int> size);
void set_location(pair<int,int> location);
void set_x_label(string x_label);
void set_y_label(string y_label);
void set_terminal_number(int terminal_number);
};
