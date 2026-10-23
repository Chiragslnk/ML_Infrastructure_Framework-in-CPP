#include<common.h>
#include<matrix.h>
#include<ml_exception.h>
#include<matrix_math.h>
#define DEVELOPMENT_MODE 1
matrix::matrix()
{
this->_rows=0;
this->_columns=0;
this->flags=0;
}

matrix::matrix(uint64_t _rows,uint64_t _columns,uint32_t flags)
{
this->flags=flags;
this->_rows=_rows;
this->_columns=_columns;
}

matrix::matrix(uint64_t _rows,uint64_t _columns)
{
this->flags=0;
this->_rows=_rows;
this->_columns=_columns;
this->collection.resize(this->_rows*this->_columns);
}

matrix::matrix(const matrix &other) // copy constructor
{
// will come in action when 
// matrix a=b is written or matrix a(b) is written
this->_rows=other._rows;
this->_columns=other._columns;
this->collection=other.collection;
}

matrix::matrix(matrix &&other) // move constructor
{
// will come in action when
// matrix a=move(b) or matrhx a(move(b)) is written
this->_rows=other._rows;
this->_columns=other._columns;
this->collection=move(other.collection);
other._rows=0;
other._columns=0;
}

matrix::~matrix()  //destructor
{
// do nothing
}

matrix & matrix::operator=(const matrix &other) // copy assignment
{
// will come in action when 
// a=b is written 
this->_rows=other._rows;
this->_columns=other._columns;
this->collection=other.collection;
return *this;  // this is important when it comes to a=b=c;
}

matrix & matrix::operator=(matrix &&other) // move assignment
{
// will come in action when
// a=move(b) 
this->_rows=other._rows;
this->_columns=other._columns;
this->collection=move(other.collection);
other._rows=0;
other._columns=0;
return *this;
}

uint64_t matrix::rows() const
{
return this->_rows;
}

uint64_t matrix::columns() const
{
return this->_columns;
}

matrix matrix::as_transposed() const
{
matrix tmp(this->_columns,this->_rows);
uint64_t r,c;
for(r=0;r<this->_rows;++r)
{
for(c=0;c<this->_columns;++c)
{
//tmp.collection[c][r]=this->collection[r][c];
tmp.collection[c*tmp._columns+r]=this->collection[r*this->_columns+c];
}
}
return tmp;
}

void matrix::save_in_csv_format(string filename) const
{
FILE  *f;
const double *ptr;
ptr=this->collection.data();
f=fopen(filename.c_str(),"wb");
if(f==NULL) throw ml_exception(string("Unable to save to ")+filename);
uint64_t r,c,c1;
c1=this->_columns-1;
for(r=0;r<this->_rows;++r)
{
for(c=0;c<c1;++c)
{
fprintf(f,"%.17g,",ptr[r*this->_columns+c]);
}
fprintf(f,"%.17g\n",ptr[r*this->_columns+c]);
}
fclose(f);
}

void matrix::save_in_binary_format(string filename) const
{
FILE  *f;
f=fopen(filename.c_str(),"wb");
if(f==NULL) throw ml_exception(string("Unable to save to ")+filename);
// storing meta collection
fwrite(&(this->_rows),sizeof(uint64_t),1,f);
fwrite(&(this->_columns),sizeof(uint64_t),1,f);
// storing  collection
fwrite(this->collection.data(),this->_rows*this->_columns*sizeof(double),1,f);
fclose(f);
}

void matrix::save(string filename,uint8_t flags) const
{
if(flags==FMT_BINARY)
{
save_in_binary_format(filename);
}else if(flags==FMT_CSV)
{
save_in_csv_format(filename);
}
else throw ml_exception("Invalid file format specified");
}

matrix matrix::operator+(const matrix &other) const
{
// validate 
if(this->_rows!=other._rows || this->_columns!=other._columns)
{
if(DEVELOPMENT_MODE)
{
throw ml_exception("Invalid matrix dimensions(" __FILE__ ":" STRINGIFY(__LINE__)  ")");
}
else
{
throw ml_exception("Invalid matrix dimensions");
}
}
matrix tmp(this->_rows,this->_columns); 
matrix_math::add(tmp.collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return tmp;
}

matrix matrix::operator-(const matrix &other) const
{
// validate 
if(this->_rows!=other._rows || this->_columns!=other._columns)
{
throw ml_exception("Invalid matrix dimensions");
}
matrix tmp(this->_rows,this->_columns); 
matrix_math::subtract(tmp.collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return tmp;
}

matrix matrix::operator*(const matrix &other) const
{
// validate 
if(this->_columns!=other._rows)
{
throw ml_exception("Invalid matrix dimensions");
}
matrix tmp(this->_rows,other._columns); 
matrix_math::multiply(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,other.collection.data(),other._rows,other._columns);
return tmp;
}

matrix matrix::operator+(double right) const
{
matrix tmp(this->_rows,this->_columns); 
matrix_math::add(tmp.collection.data(),this->collection.data(),right,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator-(double right) const
{
matrix tmp(this->_rows,this->_columns); 
matrix_math::subtract(tmp.collection.data(),this->collection.data(),right,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator*(double right) const
{
matrix tmp(this->_rows,this->_columns); 
matrix_math::multiply(tmp.collection.data(),this->collection.data(),right,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator/(double right) const
{
matrix tmp(this->_rows,this->_columns);
matrix_math::divide(tmp.collection.data(),this->collection.data(),right,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator++(void) // prefix
{
matrix_math::add(this->collection.data(),this->collection.data(),1.0,this->_rows,this->_columns);
return *this;
}

matrix matrix::operator++(int) // postfix
{
matrix tmp(this->_rows,this->_columns);
tmp=*this;
matrix_math::add(this->collection.data(),this->collection.data(),1.0,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator--()  // prefix
{
matrix_math::subtract(this->collection.data(),this->collection.data(),1.0,this->_rows,this->_columns);
return *this;
}

matrix matrix::operator--(int) // postfix
{
matrix tmp(this->_rows,this->_columns);
tmp=*this;
matrix_math::subtract(this->collection.data(),this->collection.data(),1.0,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator-=(const matrix &other)
{
// validate
if(this->_rows!=other._rows || this->_columns!=other._columns)
{
throw ml_exception("Invalid matrix dimensions");
}
matrix_math::subtract(this->collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return *this;
}

matrix matrix::operator+=(const matrix &other)
{
// validate
if(this->_rows!=other._rows || this->_columns!=other._columns)
{
throw ml_exception("Invalid matrix dimensions");
}
matrix_math::add(this->collection.data(),this->collection.data(),other.collection.data(),this->collection.size());
return *this;
}

matrix matrix::operator*=(const matrix &other)
{
if(this->_columns!=other._rows || other._rows!=other._columns)
{
throw ml_exception("Invalid matrix dimensions");
}
matrix tmp(this->_rows,other._columns); 
matrix_math::multiply(tmp.collection.data(),this->collection.data(),this->_rows,this->_columns,other.collection.data(),other._rows,other._columns);
*this=tmp;
return *this;
}

matrix matrix::operator+=(double value)
{
matrix_math::add(this->collection.data(),this->collection.data(),value,this->_rows,this->_columns);
return *this;
}

matrix matrix::operator-=(double value)
{
matrix_math::subtract(this->collection.data(),this->collection.data(),value,this->_rows,this->_columns);
return *this;
}

matrix matrix::operator*=(double value)
{
matrix_math::multiply(this->collection.data(),this->collection.data(),value,this->_rows,this->_columns);
return *this;
}

matrix matrix::operator/=(double value)
{
matrix_math::divide(this->collection.data(),this->collection.data(),value,this->_rows,this->_columns);
return *this;
}

matrix matrix::operator-() const
{
matrix tmp(this->_rows,this->_columns); 
matrix_math::multiply(tmp.collection.data(),this->collection.data(),-1.0,this->_rows,this->_columns);
return tmp;
}

matrix matrix::operator^(uint32_t value)  const
{
if(this->_rows!=this->_columns)
{
throw ml_exception("Not a square matrix, hence cannot create identity matrix");
}
if(value==0) 
{
matrix tmp; // internal vector is empty
vector<double> v(this->_rows*this->_columns,0.0);
uint64_t sz=this->_rows*this->_columns;
for(uint64_t i=0;i<sz;i+=(this->_columns+1)) v[i]=1.0; 
tmp.collection=move(v);
return tmp;
}
if(value==1) return *this;
uint32_t p;
matrix tmp=*this;
for(p=1;p<value;++p) tmp*=(*this);
return tmp;
}

void matrix::set(uint64_t row_index,uint64_t column_index,double value)
{
if(row_index>=this->_rows || column_index>=this->_columns)
{
throw ml_exception("Index out of bounds");
}
if(flags & IDENTITY)  // bitwise anding operation
{
if(value==0.0 && row_index!=column_index) return;
else if(value==1.0 && row_index==column_index) return;
// remove identity flag
this->flags=this->flags ^ IDENTITY;
this->collection.resize(this->_rows * this->_columns);
// set everything to 0.0
memset(this->collection.data(),0,this->collection.size()*sizeof(double));
// set diagonals to 1.0
for(uint64_t r=0;r<_rows;++r) this->collection[r*this->_columns+r]=1.0;
}
this->collection[row_index*this->_columns+column_index]=value;
}

double matrix::get(uint64_t row_index,uint64_t column_index) const 
{
if(row_index>=this->_rows || column_index>=this->_columns)
{
throw ml_exception("Index out of bounds");
}
if(flags & IDENTITY)  // bitwise anding operation
{
if(row_index==column_index)
{
return 1.0;
}
else
{
return 0.0;
}
}
return this->collection[row_index*this->_columns+column_index];
}

matrix matrix::identity(uint64_t rows)
{
uint32_t flags;
flags=IDENTITY;
uint64_t columns;
columns=rows;
matrix tmp(rows,columns,flags);
return tmp;
}

matrix::matrix_row matrix::operator[](uint64_t row_index)
{
if(row_index>=this->_rows)
{
throw ml_exception("Index out of bounds");
}
return matrix::matrix_row(this,row_index); // anonymous object returned
}

matrix::matrix_row::matrix_row(matrix *mat,uint64_t row_index)
{
this->mat=mat;
this->row_index=row_index;
}

matrix::matrix_cell::matrix_cell(matrix *mat,uint64_t row_index,uint64_t column_index)
{
this->mat=mat;
this->row_index=row_index;
this->column_index=column_index;
}

matrix::matrix_cell::operator double() const
{
return this->mat->get(this->row_index,this->column_index);
}

matrix::matrix_cell & matrix::matrix_cell::operator=(double value)
{
this->mat->set(this->row_index,this->column_index,value);
return *this;
}

matrix::matrix_cell matrix::matrix_row::operator[](uint64_t column_index)
{
if(column_index>=mat->_columns)
{
throw ml_exception("Index out of bounds");
}
return matrix::matrix_cell(this->mat,this->row_index,column_index);
}

// independent functions  start here
matrix operator+(double left,const matrix &right)
{
matrix tmp(right._rows,right._columns);
matrix_math::add(tmp.collection.data(),left,right.collection.data(),right._rows,right._columns);
return tmp;
}

matrix operator-(double left,const matrix &right)
{
matrix tmp(right._rows,right._columns);
matrix_math::subtract(tmp.collection.data(),left,right.collection.data(),right._rows,right._columns);
return tmp;
}

matrix operator*(double left,const matrix &right)
{
matrix tmp(right._rows,right._columns);
matrix_math::multiply(tmp.collection.data(),left,right.collection.data(),right._rows,right._columns);
return tmp;
}

matrix operator/(double left,const matrix &right)
{
matrix tmp(right._rows,right._columns);
matrix_math::divide(tmp.collection.data(),left,right.collection.data(),right._rows,right._columns);
return tmp;
}

