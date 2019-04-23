#include "PRIORITY_QUEUE.h"

PRIORITY_QUEUE::PRIORITY_QUEUE(int size)
{
	numItems = 0;
	maxSize = size;
	items = new HUFFNode[maxSize];
	rear = -1;
}

PRIORITY_QUEUE::~PRIORITY_QUEUE()
{
	delete[]items;
}

bool PRIORITY_QUEUE::isEmpty()
{
	if (rear == -1)
	{
		return true;
	}
	return false;
}

bool PRIORITY_QUEUE::enQueue(HUFFNode newItem)
{
	if (rear == maxSize) 
		return false;
	numItems++;
	rear++;
	items[rear] = newItem;
	heapifyUp(rear);
	return true;
}

bool PRIORITY_QUEUE::deQueue(HUFFNode & item)
{
	if (isEmpty())
	{
		item.freq = 0;
		return false;
	}
	item = items[0];
	items[0] = items[rear];
	rear--;
	heapifyDown(0);
	return true;
}

void PRIORITY_QUEUE::heapifyUp(int i)
{
	HUFFNode temp;
	while (i>0&& (items[i].freq <= items[(i - 1) / 2].freq))
	{
		temp = items[i];
		items[i] =items[(i-1) / 2];
		items[(i-1) / 2] = temp;
		i = (i - 1) / 2;
		if (items[i].freq >= items[(i - 1) / 2].freq)
			break;
		
	}
}

void PRIORITY_QUEUE::heapifyDown(int i)
{
	HUFFNode temp;
	while (i<=rear/2 )
	{
		int child = 2 * i + 1;
		if (child < rear - 1)
		{
			if (items[child].freq > items[child+1].freq)
				child++;
		}
		if (items[i].freq <= items[child].freq)
			break;
		temp = items[i];
		items[i] = items[child];
		items[child] = temp;
		i = child;

	}
}

int PRIORITY_QUEUE::getNumItems()
{
	return numItems;
}
