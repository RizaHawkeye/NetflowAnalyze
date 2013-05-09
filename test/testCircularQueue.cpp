#include "../CircularQueue.h"
#include <iostream>

using namespace std;

int main()
{
	int buf1[6] = {0,1,2,3,4,5};
	int buf2[6] = {6,7,8,9,10,11};
	CircularQueue<int> cq(10);
	int num1 = cq.enQueue(buf1,6);
	num1 = cq.enQueue(buf2,6);
	num1 = cq.enQueue(buf2,4);
	num1 = cq.enQueue(buf2,3);
	
	int* buf = 0;
	int num2 = cq.deQueue(buf,7);
	num1 = cq.enQueue(buf1,3);
	num2 =cq.deQueue(buf,6);

	return 0;
}
