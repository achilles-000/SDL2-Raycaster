#include <stdio.h>
#include <stdbool.h>
#include <math.h>

int main(){
    double x = 0.0;
    double result = 0.0;
    double result1 = 0.0;
    

    for(int i = 0; i < 100; i++){
        x += 1;
        result = 50*sin(x);
        result1 = 50*cos(x);
        printf("%f sin: ", result);
        printf("%f cos: ", result1);
    }

}