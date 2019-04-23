#pragma once
#include <iostream>
using namespace std;
struct HUFFNode {

	unsigned char	c;
	int		freq;
	int		nLeft;
	int		nRight;

};

class PRIORITY_QUEUE
{
public:
	PRIORITY_QUEUE(int size);
	~PRIORITY_QUEUE();

	int getNumItems();
	bool isEmpty();
	bool enQueue(HUFFNode newItem);
	bool deQueue(HUFFNode &item);
private:
	HUFFNode *items;
	int rear;
	int maxSize;
	void heapifyUp(int i);
	void heapifyDown(int i);
	int numItems;

};