#include<csv.h>
#include<cstdint>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>
#include<ml_exception.h>
#define BUFFER_SIZE 4096 
void csv::load_x_y(string filename,matrix &x,matrix &y,uint32_t skip_lines)
{
unsigned char buffer[BUFFER_SIZE];
uint64_t bytes_left_to_read;
uint64_t bytes_fetched;
uint64_t row_index;
uint64_t column_index;
double *x_ptr;
double *y_ptr;
char str[101];
uint32_t index;
uint8_t i;
unsigned char m;
double value;
unsigned char last_char;
uint32_t columns;
uint64_t rows;
uint64_t file_size; 
uint64_t pos; // to store position of first byte of first data row
struct stat s;
if(access(filename.c_str(),R_OK))
{
throw ml_exception(string("Cannot access ")+filename);
}
if(stat(filename.c_str(),&s)<0)
{
throw ml_exception(string("Cannot gather information of ")+filename);
}
file_size=s.st_size;
if(file_size==0)
{
throw ml_exception(string("Empty ")+filename);
}
int file_descriptor; 
file_descriptor=open(filename.c_str(),O_RDONLY | O_BINARY);
if(file_descriptor<0)
{
throw ml_exception(string("Cannot open ")+filename);
}
bytes_left_to_read=file_size;
columns=0;
rows=0;
pos=0; // important, when skip_lines is zero
while(bytes_left_to_read>0)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
for(index=0;index<bytes_fetched;++index)
{
m=buffer[index];
if(m=='\n')
{
++rows;
if(rows==skip_lines)
{
pos=file_size-bytes_left_to_read+index+1;
}
}
else if(m==',' && rows==skip_lines)
{
++columns;
}
}
last_char=m;
bytes_left_to_read-=bytes_fetched;
}
if(last_char!='\n') ++rows;
if(rows<=skip_lines)
{
close(file_descriptor); 
throw ml_exception(filename+string(" does not contain data"));
}
rows-=skip_lines;
++columns;
x.flags=0;
x._rows=rows;
x._columns=columns;
x.collection.resize(x._rows*x._columns);
y.flags=0;
y._rows=rows;
y._columns=1;
y.collection.resize(y._rows*y._columns);
auto sz=x.collection.size();
x_ptr=x.collection.data();
y_ptr=y.collection.data(); 
for(index=0;index<sz;index+=x._columns) x_ptr[index]=1.0;
// in we file we at the end(after last byte)
lseek(file_descriptor,pos,SEEK_SET);
bytes_left_to_read=file_size-pos;
row_index=0;
column_index=1;
i=0;
while(bytes_left_to_read>0)
{
bytes_fetched=read(file_descriptor,buffer,BUFFER_SIZE);
index=0;
while(index<bytes_fetched)
{
m=buffer[index];
if(m!=',' && m!='\n')
{
str[i++]=m; // note, it should not be prefix
}
else
{
str[i]='\0';
i=0;
value=atof(str);
if(m==',')
{
x_ptr[row_index*x._columns+column_index]=value;
column_index++;
}
else // \n part
{
y_ptr[row_index++]=value;
column_index=1;
}
}
++index;
}
bytes_left_to_read-=bytes_fetched;
}
if(last_char!='\n')
{
str[i]='\0';
value=atof(str);
y_ptr[row_index]=value;
}
close(file_descriptor);
}

