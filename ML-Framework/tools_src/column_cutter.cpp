#include<set>
#include<unordered_map>
#include<algorithm>
#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<sys\stat.h>
#include<cstdint>
#include<vector>
#include<unordered_set>
using namespace std;
#define BUFFER_SIZE 8192
void clear_stdin_buffer()
{
while(getchar()!='\n'); // to clear buffer
}
int main()
{
uint64_t bytes_fetched;
char yn;
char buffer[BUFFER_SIZE]; // 8KB
FILE *f;
char *header;
char filename[4096];
printf("CSV Column Cutter\n\n");
printf("Enter csv dataset filename : ");
scanf("%[^\n]s",filename);
clear_stdin_buffer();
struct stat s;
if(stat(filename,&s)<0)
{
printf("CSV dataset (%s) does not exist\n",filename);
return EXIT_FAILURE;
}
if(s.st_size==0)
{
printf("csv dataset (%s) is empty\n",filename);
return EXIT_FAILURE;
}
f=fopen(filename,"rb"); // b very very important
if(f==NULL)
{
printf("Unable to open (%s)\n",filename);
return EXIT_FAILURE;
}
printf("Display first few lines (Y/N) : ");
yn=getchar();
clear_stdin_buffer();
if(yn=='y') yn='Y';
uint64_t line;
int index;
uint32_t column_count=1;
int number_of_lines_to_display=5;
if(yn!='Y') number_of_lines_to_display=1;
char ch;
uint32_t header_size=0;
while(1)
{
bytes_fetched=fread(buffer,sizeof(char),BUFFER_SIZE,f); 
for(index=0;index<bytes_fetched && buffer[index]!='\n';++index);
header_size+=index;
if(index<bytes_fetched) break;
if(feof(f)) break;
}
printf("Size of header %d\n",header_size);
header=(char *)malloc(sizeof(char)*(header_size+1));
rewind(f);
line=1;
uint64_t bytes_to_read=s.st_size;
int start;
int flag=0;
uint32_t header_index=0;
while(bytes_to_read>0)
{
bytes_fetched=fread(buffer,sizeof(char),BUFFER_SIZE,f);
start=0;
for(index=0;index<bytes_fetched;++index)
{
ch=buffer[index];
if(ch==',' && line==1) ++column_count;
if(ch=='\n' || index==bytes_fetched-1)
{
if(yn=='Y') write(STDOUT_FILENO,buffer+start,index-start+1);
start=index+1;
if(ch=='\n' && line==number_of_lines_to_display)
{
flag=1;
break;
}
if(ch=='\n') ++line;
}
if(line==1) header[header_index++]=ch;
}
if(flag==1) break;
bytes_to_read-=bytes_fetched;
} // infinite loop ends
header[header_size-1]='\0';
char last_char=buffer[index-1];
if(last_char!='\n') printf("\n");
//printf("Columns : %d\n",column_count);
if(column_count==1)
{
printf("CSV dataset (%s) has only one columns, hence cutting column doesn't make sense\n",filename);
return EXIT_SUCCESS;
}
int choice;
int column_number;
while(1)
{
printf("Number of columns in (%s) : (%d)\n",filename,column_count);
printf("1. Cut by specifying column names\n");
printf("2. Cut by speicfying column numbers\n");
printf("Enter your choice : ");
scanf("%d",&choice);
clear_stdin_buffer();
if(choice<1 || choice>2)
{
printf("Invalid choice\n");
continue;
}
break;
}
char *columns_to_cut;
columns_to_cut=(char *)malloc(header_size+1);
set<uint32_t> column_numbers;
if(choice==1)
{
vector<int> base_indexes;
printf("Header in csv is\n");
printf("%s\n",header);
printf("\n"); 
printf("Enter comma separated, column names to cut : ");
scanf("%[^\n]s",columns_to_cut);
clear_stdin_buffer(); 
for(index=0;columns_to_cut[index]!='\0' && (columns_to_cut[index]==' ' || columns_to_cut[index]=='\t');++index);
if(columns_to_cut[index]=='\0')
{
free(header);
free(columns_to_cut);
fclose(f);
printf("No column names specified for cutting\n");
return EXIT_FAILURE;
}
base_indexes.push_back(0);
for(index=0;columns_to_cut[index]!='\0';++index)
{
if(columns_to_cut[index]==',')
{
columns_to_cut[index]='\0'; 
if(columns_to_cut[index+1]!='\0') base_indexes.push_back(index+1);
}
}
/*for(auto j:base_indexes)
{
printf("Column name [%s]\n",columns_to_cut+j);
}*/
// create set from columns to cut
size_t sz=base_indexes.size();
int last_index=sz-1;
int start_index,end_index;
unordered_set<string> input_column_names;
for(index=0;index<sz;++index)
{
start_index=base_indexes[index]; 
while(columns_to_cut[start_index]==' ' || columns_to_cut[start_index]=='\t')
{
++start_index;
}
if(columns_to_cut[start_index]=='\0') continue;
if(index<last_index)
{
end_index=base_indexes[index+1]-2;
}
else
{	
for(end_index=start_index+1;columns_to_cut[end_index]!='\0';++end_index);
--end_index;
}	
for(;columns_to_cut[end_index]==' ' || columns_to_cut[end_index]=='\t';--end_index); 	
string str(columns_to_cut+start_index,end_index-start_index+1); 
transform(str.begin(),str.end(),str.begin(),[](char m){ return (m>=97 && m<=122)?m-32:m; });
input_column_names.insert(str);
}
// printing the contents of columns_to_cut in set 
for(auto s:input_column_names) printf("%s\n",s.c_str()); // header to map (key - value pair) key : COLUMN NAME
// value : vector of pairs, every pair will contain
// original column name and column number 
unordered_map<string,vector<pair<string,uint32_t>>> header_map;
printf("Header (%s)\n",header);
int start=0;
int current=0;
column_number=1;
while(1)
{
while(header[current]!=',' && header[current]!='\0') ++current;
string original(header+start,current-start);
string upper_cased=original; 
transform(upper_cased.begin(),upper_cased.end(),upper_cased.begin(),[](char m){ return (m>=97 && m<=122)?m-32:m; });
cout<<original<<" --- "<<upper_cased<<endl; 
header_map[upper_cased].push_back({original,column_number});
if(header[current]=='\0') break;
++column_number;
start=current+1;
current=start;
}
// iterate set and search and validate and keep creating
// vector of columns_numbers to cut
for(auto s:input_column_names)
{
auto iter=header_map.find(s);
if(iter==header_map.end())
{
cout<<"Column : "<<s<<" does not exist"<<endl;
free(header);
fclose(f);
return EXIT_FAILURE;
}
// correct name
for(auto pp:iter->second)
{
column_numbers.insert(pp.second);
}
}
} // choice is 1, if ends
if(choice==2)
{
printf("Number of columns : %d\n",column_count); 
printf("Enter comma separated, column numbers to cut (%d-%d) : ",1,column_count);
scanf("%[^\n]s",columns_to_cut);
clear_stdin_buffer();
int k;
int acc;
char cc;
acc=0;
k=0;
while(1)
{
cc=columns_to_cut[k++];
if(cc==',' || cc=='\0')
{
if(acc==0 || acc>column_count)
{
cout<<"Invalid input"<<endl;
free(header);
fclose(f);
return EXIT_FAILURE;
}
column_numbers.insert(acc);
acc=0;
if(cc=='\0') break;
}
else
{
if(cc<48 || cc>57)
{
cout<<"Invalid input"<<endl;
free(header);
fclose(f);
return EXIT_FAILURE;
}
acc=acc*10+(cc-48);
}
}
}
// over choice may be 1 or 2, but we have vector of column numbers to be cut
for(auto cn:column_numbers) cout<<cn<<endl;
printf("Cut columns from %s ? ",filename);
char create_new_file_yn=getchar();
clear_stdin_buffer();
char new_name[4096]; 
if(create_new_file_yn!='Y' && create_new_file_yn!='y')
{
printf("Enter name of the file to create after cutting columns from %s : ",filename);
scanf("%[^\n]s",new_name);
clear_stdin_buffer();
}
else
{
strcpy(new_name,"tmp.tmp");
}
// open new_file in (w) mode
/*
FILE *f2=fopen(new_file,"w");
if(f2==NULL)
{
cout<<"Unable to create file"<<endl;
free(header);
fclose(f);
return EXIT_FAILURE;
}
rewind(f);
bytes_to_read=s.st_size;
column_number=1;
while(bytes_to_read>0)
{
bytes_fetched=fread(buffer,sizeof(char),BUFFER_SIZE,f);
start=0;
current=0;
// code to iterate on buffer starts here
	if char at index is ',' or '\n' 		
	if the said column is to be wrriten, 
	write it using start and current
	if char at index is '\n' then column_number=1
// code to iterate on buffer ends here
	// if something is left in buffer which not yet been
	// written, write it, if it is supposed to be written
}
if user said that new file is not to be created
	open original file in "w and new_file in "r" mode
	copy everything back from new file to original file
*/
free(header);
fclose(f);
return EXIT_SUCCESS;
}
