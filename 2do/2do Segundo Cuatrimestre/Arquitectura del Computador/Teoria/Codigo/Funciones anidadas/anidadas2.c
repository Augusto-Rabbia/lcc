//Función anidada - https://es.qaz.wiki/wiki/Nested_function
#include <stdio.h>

void
sort (int *items, int size)
{
  void quickSort (int first, int last)
  {
    void swap (int p, int q)
    {
      int tmp = items[p];
      items[p] = items[q];
      items[q] = tmp;
    } int partition ()
    {
      int pivot = items[first], index = first;
      swap (index, last);
      for (int i = first; i < last; i++)
	if (items[i] < pivot)
	  swap (index++, i);
      swap (index, last);
      return index;
    }
    if (first < last)
      {
	int pivotIndex = partition ();
	quickSort (first, pivotIndex - 1);
	quickSort (pivotIndex + 1, last);
      }
  } quickSort (0, size - 1);
}

int main(){
	int a[10]={3,4,6,2,4,1,5,8,0,9};
	sort(a,10);
	for(int i=0;i<10;i++){
		printf("%d ",a[i]);
	}
	printf("\n");
	return 0;
}