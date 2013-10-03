#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

int main() {

ifstream file("test-matrix125");
clock_t t = clock();

while(!file.eof()) {
double value;
file >> value;
if(((clock() - t)/ CLOCKS_PER_SEC) > 15) {
	clock_t intert = clock();
	cout << (int) file.tellg();
	cout << "time " << (clock() - intert)/CLOCKS_PER_SEC << " done" ;
 	break;
}
}

/*while(!file.eof()){
file.get();
}*/

cout << (clock() - t)/CLOCKS_PER_SEC;
file.close();
return 0;
}
