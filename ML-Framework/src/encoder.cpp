#include<sys/stat.h>
#include<cstdint>
#include<encoder.h>
#include<unordered_set> 
void encoder::encode_one_hot(char *source,char *target,vector<uint32_t> columns_to_encode,bool has_header)
{
if(target==NULL || source==NULL)
{
throw invalid_argument("Source and target required");
}
if(columns_to_encode.size()==0)
{
throw invalid_argument("No columns to encode");
}
struct stat s;
if(stat(source,&s)<0)
{
throw invalid_argument("Missing source file");
}
FILE *source_file=fopen(source,"rb");
if(source_file==NULL)
{
throw invalid_argument("Unable to open source file");
}
FILE *target_file=fopen(target,"wb");
if(target_file==NULL)
{
fclose(source_file);
throw invalid_argument("Unable to open target file");
}
char buffer[4096];
uint64_t source_size=s.st_size;
uint64_t bytes_left_to_read=source_size;
uint64_t bytes_read;
uint64_t idx;
uint64_t columns=0;
uint64_t rows=0;
uint64_t start;
char ch;
uint32_t column_index;
string word;
vector<pair<uint64_t,uint32_t>> line_info;
uint64_t pos=0;
uint64_t prev_pos=0;
uint32_t max_line_len=0;
uint32_t line_len;
while(bytes_left_to_read>0)
{
bytes_read=fread(buffer,sizeof(char),4096,source_file);
bytes_left_to_read-=bytes_read;
for(idx=0;idx<bytes_read;++idx)
{
ch=buffer[idx];
if(!has_header && ch==',' && rows==0) ++columns; 
else if(has_header && ch==',' && rows==1) ++columns;
if(ch=='\n')
{
line_len=pos-prev_pos;
if(line_len>max_line_len) max_line_len=line_len;
line_info.push_back({prev_pos,line_len});
prev_pos=pos+1;
if(!has_header && rows==0) ++columns;
else if(has_header && rows==1) ++columns;
++rows;
}
pos++;
} // for ends
} // while ends
if(ch!='\n') // last char is not \n
{
line_len=pos-prev_pos;
if(line_len>max_line_len) max_line_len=line_len;
line_info.push_back({prev_pos,line_len});
}
if(has_header) --rows;
//cout<<"rows : "<<rows<<endl;
//cout<<"columns : "<<columns<<endl;
//cout<<"fattest line : "<<max_line_len<<endl;
if(rows==0)
{
fclose(source_file);
fclose(target_file);
throw invalid_argument("No rows in dataset");
}
for(auto cn:columns_to_encode)
{
if(cn>=columns)
{
fclose(source_file);
fclose(target_file); 
throw invalid_argument("Invalid column number provided to encode");
}
}
char *line=new char[max_line_len+1];
char **ptr=new char *[columns];
idx=0;
if(has_header) idx=1;
auto sz=line_info.size();
pair<uint64_t,uint32_t> p;
int i,j; 
vector<unordered_set<string>> unique_strings_sets(columns_to_encode.size());
while(idx<sz)
{
p=line_info[idx];
fseek(source_file,p.first,SEEK_SET);
fread(line,sizeof(char),p.second,source_file);
i=p.second-1;
while(line[i]=='\r' || line[i]=='\n') --i;
line[i+1]='\0';
j=0;
ptr[j++]=&line[0];
for(i=0;line[i]!='\0';++i)
{
if(line[i]==',')
{
line[i]='\0';
ptr[j++]=&line[i+1];
}
}
// now we have line splits against collection of ptr
j=0;
for(auto cte:columns_to_encode)
{
unique_strings_sets[j++].insert(string(ptr[cte]));
}
idx++;
}
uint32_t new_column_count=columns-columns_to_encode.size(); 
for(auto s:unique_strings_sets) new_column_count+=s.size(); 
//cout<<"New column count is : "<<new_column_count<<endl;
vector<vector<string>> headings;
if(has_header)
{
headings.resize(columns_to_encode.size());
p=line_info[0];
fseek(source_file,p.first,SEEK_SET);
fread(line,sizeof(char),p.second,source_file);
i=p.second-1;
while(line[i]=='\r' || line[i]=='\n') --i;
line[i+1]='\0';
j=0;
ptr[j++]=&line[0];
for(i=0;line[i]!='\0';++i)
{
if(line[i]==',') 
{
line[i]='\0';
ptr[j++]=&line[i+1];
}
}
j=0;
for(auto cte:columns_to_encode)
{
for(auto wrd:unique_strings_sets[j])
{
headings[j].push_back(ptr[cte]+string("_")+wrd);
}
++j;
}
// write new headings in target file
int r=0;
j=0;
int k=0;
for(auto cte:columns_to_encode)
{
while(k<cte)
{
if(r<new_column_count-1)
{
fprintf(target_file,"%s,",ptr[k]);
}
else
{
fprintf(target_file,"%s",ptr[k]);
}
++r;
++k;
}
k++;
for(auto hd:headings[j])
{
if(r<new_column_count-1)
{
fprintf(target_file,"%s,",hd.c_str());
}
else
{
fprintf(target_file,"%s",hd.c_str());
}
++r;
}
j++;
}
while(k<columns)
{
if(r<new_column_count-1)
{
fprintf(target_file,"%s,",ptr[k]);
}
else
{
fprintf(target_file,"%s",ptr[k]);
}
++r;
++k;
}
fputc('\n',target_file);
} // headings created if exist
idx=0;
if(has_header) idx=1;
// writing data row starts
while(idx<sz)
{
p=line_info[idx];
fseek(source_file,p.first,SEEK_SET);
fread(line,sizeof(char),p.second,source_file);
i=p.second-1;
while(line[i]=='\r' || line[i]=='\n') --i;
line[i+1]='\0';
j=0;
ptr[j++]=&line[0];
for(i=0;line[i]!='\0';++i)
{
if(line[i]==',')
{
line[i]='\0';
ptr[j++]=&line[i+1];
}
}
// now we have line splits against collection of ptr 
// // time to write the splits, but replace the ones that are part of columns_to_encode
string data;
int r=0;
int k=0;
for(j=0;j<columns;++j)
{
if(columns_to_encode[r]!=j)
{
fprintf(target_file,"%s",ptr[j]);
if(k<new_column_count-1) fputc(',',target_file);
k++;
}
else
{
for(auto wrd:unique_strings_sets[r])
{
if(strcmp(wrd.c_str(),ptr[j])==0) fputc('1',target_file); 
else fputc('0',target_file);
if(k<new_column_count-1) fputc(',',target_file);
k++;
}
r++;
}
}
fputc('\n',target_file);
idx++;
}
// writing data rows ends
delete [] line;
delete [] ptr;
fclose(source_file);
fclose(target_file);
}
void encoder::encode_one_hot(char *source,char *target,vector<string> columns_to_encode)
{
// do it yourself, determine the column numbers and create vector and // call the implemented method and pass true for header
}
