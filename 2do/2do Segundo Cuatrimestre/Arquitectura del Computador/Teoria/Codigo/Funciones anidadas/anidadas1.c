// Función anidada - https://es.qaz.wiki/wiki/Nested_function
#include <stdio.h>

float E(float x) {
	float F(float y) {
		return x + y; }
	
	return F(3) + F(4); 
}

int main(){
	printf("%f\n", E(10.0));
}
