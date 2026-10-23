#include <iostream>
#include <ml.h>
using namespace std;

int test_add_value()
{
    matrix b(5,5);
    matrix a;
    int rr,cc;
    double value1=1.0;
    
    cout << "[DEBUG] Loop start hone wala hai..." << endl;
    for(rr=0;rr<5;++rr) {
        for(cc=0;cc<5;++cc) {
            b.set(rr,cc,value1++);
        }
    }
    cout << "[DEBUG] Loop khatam. Ab matrix addition hone wala hai..." << endl;
    
    // Yahan crash hone ke 99% chances hain
    a = b + 20.3; 
    
    cout << "[DEBUG] Matrix addition successful! Ab print karenge..." << endl;
    for(rr=0;rr<5;++rr) {
        for(cc=0;cc<5;++cc) {
            cout<<a.get(rr,cc)<<" ";
        }
        cout<<endl;
    }
    return 0;
}

int main() {
    test_add_value();
    return 0;
}
