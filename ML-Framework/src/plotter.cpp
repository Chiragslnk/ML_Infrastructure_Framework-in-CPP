/*the instructions sent to gnuplot are accoriding to windows
for linux you will have to set terminial x11
and use popen instead of _popen on windows install gnuplot and see to it that the folder that contains the gnuplot.exe
is added to path environment variable
*/
#include<plotter.h>
shape_point * point(double x,double y)
{
shape_point *point=(shape_point *)malloc(sizeof(shape_point));
if(point==NULL) return NULL;
point->type='P';
point->x=x;
point->y=y;
return point;
}
shape_line * line(shape_point *point_1,shape_point *point_2)
{
shape_line * line;
line=(shape_line *)malloc(sizeof(shape_line));
if(line==NULL) return NULL;
line->type='L';
line->point_1=point_1;
line->point_2=point_2;
return line;
}
plotter::plotter()
{
this->gnuplot=NULL;
this->x_range={0,100};
this->y_range={0,100};
this->size={600,600};
this->location={10,10};
this->terminal_number=1;
}
void plotter::remove(char type)
{
int idx=0;
while(idx<this->shapes.size())
{
if(this->shapes[idx]->type==type) this->shapes.erase(this->shapes.begin()+idx);
else ++idx;
}
}
void plotter::add(shape_point *point)
{
this->shapes.push_back((shape *)point);
}
void plotter::add(shape_line *line)
{
this->shapes.push_back((shape *)line);
}
void plotter::set_size(pair<int,int> size)
{
this->size=size;
}
void plotter::set_location(pair<int,int> location)
{
this->location=location;
}
void plotter::set_x_label(string x_label)
{
this->x_label=x_label;
}
void plotter::set_y_label(string y_label)
{
this->y_label=y_label;
}
void plotter::set_x_range(pair<int,int> x_range)
{
this->x_range=x_range;
}
void plotter::set_y_range(pair<int,int> y_range)
{
this->y_range=y_range;
}
void plotter::set_terminal_number(int terminal_number)
{
this->terminal_number=terminal_number;
}
void plotter::clear()
{
this->shapes.clear();
// code to clear graph
}
void plotter::set_title(string title)
{
this->title=title;
}
void plotter::plot()
{
// iterate and plot graph using gnuplot
char command[101];
if(gnuplot==NULL)
{
gnuplot=_popen("gnuplot --persist","w");
sprintf(command,"set terminal windows %d wsize %d,%d position %d,%d\n",this->terminal_number,this->size.first,this->size.second,this->location.first,this->location.second);
fprintf(gnuplot,command);
fflush(gnuplot);
fprintf(gnuplot,"unset warnings\n");
fflush(gnuplot); 
sprintf(command,"set title \"%s\"\n",this->title.c_str());
fprintf(gnuplot,command);
fflush(gnuplot); 
sprintf(command,"set xlabel '%s'\n",this->x_label.c_str());
fprintf(gnuplot,command);
fflush(gnuplot); 
sprintf(command,"set ylabel '%s'\n",this->y_label.c_str());
fprintf(gnuplot,command);
fflush(gnuplot);
fprintf(gnuplot,"set grid\n");
fflush(gnuplot); 
sprintf(command,"set xrange[%d:%d]\n",this->x_range.first,this->x_range.second);
fprintf(gnuplot, command);
fflush(gnuplot); 
sprintf(command,"set yrange[%d:%d]\n",this->y_range.first,this->y_range.second);
fprintf(gnuplot, command);
fflush(gnuplot);
}
shape_point *point_ptr;
shape_line *line_ptr;
int line_count,point_count;
line_count=point_count=0;
for(auto s:this->shapes)
{
if(s->type=='P') ++point_count;
else if(s->type=='L') ++line_count;
}
if(point_count>0 && line_count>0)
{
fprintf(gnuplot,"plot '-' with points pointtype 7 pointsize 0.2,'-' with lines linewidth 3 linecolor rgb 'red'\n");
fflush(gnuplot);
}
else if(point_count>0)
{
fprintf(gnuplot,"plot '-' with points pointtype 7 pointsize 0.2\n");
fflush(gnuplot);
}else if(line_count>0)
{
fprintf(gnuplot,"plot '-' lines linewidth 3 linecolor rgb 'red'\n");
fflush(gnuplot);
}
for(shape *shape_ptr:this->shapes)
{
if(shape_ptr->type=='P')
{
point_ptr=(shape_point *)shape_ptr; 
fprintf(gnuplot,"%f %f\n", point_ptr->x, point_ptr->y);
fflush(gnuplot);
}
}
if(point_count>0)
{
fprintf(gnuplot,"e\n");
fflush(gnuplot);
}
for(shape *shape_ptr:this->shapes)
{
if(shape_ptr->type=='L')
{
line_ptr=(shape_line *)shape_ptr; 
fprintf(gnuplot,"%f %f\n", line_ptr->point_1->x, line_ptr->point_1->y);
fflush(gnuplot); 
fprintf(gnuplot,"%f %f\n", line_ptr->point_2->x, line_ptr->point_2->y);
fflush(gnuplot);
}
}
if(line_count>0)
{
fprintf(gnuplot,"e\n");
fflush(gnuplot);
}
}
