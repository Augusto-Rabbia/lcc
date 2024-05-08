#include <stdio.h>
typedef struct s_bits
{
	unsigned int mostrar: 1;
unsigned int rojo: 8;
unsigned int azul: 8;
unsigned int verde: 8;
unsigned int transparencia: 1;
}campo_bits;

int main(){
campo_bits unColor;
/* Se crea un color verde */
unColor.rojo = 51;
unColor.azul = 55;
unColor.verde = 255;
unColor.transparencia = 1;
/* Se verifica si el color es transparente */
if (unColor.transparencia == 1)
{
		printf("El color es transparente\n");
	}
	return 0;
}