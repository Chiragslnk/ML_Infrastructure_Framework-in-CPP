#include<ml_exception.h>
using namespace std;
ml_exception::ml_exception(string msg):runtime_error(msg)
{
}
