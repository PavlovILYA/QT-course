#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    while(1)
    {
        //fprintf(stdout, "Hello, World! It's a message to STDOUT!\n");
        cout << "Hello, World! It's a message to STDOUT!" << endl;
        sleep(3);
        //fprintf(stderr, "Good bye, World! It's a message to STDERR!\n");
        cerr << "Good bye, World! It's a message to STDERR!" << endl;
        sleep(3);
    }
    return 0;
}
