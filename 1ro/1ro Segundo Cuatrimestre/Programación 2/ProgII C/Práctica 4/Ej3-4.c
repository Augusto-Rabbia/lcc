#include <stdio.h>
#include <stdlib.h>

struct Point2D{
    int x;
    int y;
};
struct Rectangle{
    int NWPoint[2];
    int SEPoint[2];
};

int inRect(struct Point2D P, struct Rectangle Rect){
    if(Rect.NWPoint[0] < P.x && P.x < Rect.SEPoint[0] && Rect.SEPoint[1] < P.y && P.y < Rect.NWPoint[1]){
        return 1;
    }
    else return 0;
}
int main(){
    struct Point2D P1;
    P1.x = 5, P1.y = 1;
    
    struct Rectangle Rect1;
    Rect1.NWPoint[0] = 0, Rect1.NWPoint[1] = 2;
    Rect1.SEPoint[0] = 2, Rect1.SEPoint[1] = 0;

    if(inRect(P1, Rect1)){
        printf("Yes");
    }
    else {printf("No");}


    return 0;
}