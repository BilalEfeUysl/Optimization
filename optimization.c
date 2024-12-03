#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 24 // Görüntü boyutu NxN

void read_csv(const char *filename, double matrix[200][577]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Dosya acilamadi");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 577; j++) {
            if (fscanf(file, "%lf,", &matrix[i][j]) != 1) {
                fprintf(stderr, "Dosya formati hatali veya beklenmeyen veri\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

void generate_w(double w[577],double baslangic){

    for(int i=0;i<577;i++){
        w[i] = baslangic;
    }

}

double loss_türevi(double w,double x,int hedef){

    return -2*(hedef-tan(w*x))*(1-pow(tan(w*x),2))*x;


}

void hedef_duzenleme(int hedef[200]){

    for(int i=0;i<100;i++){
        hedef[i] = 1;
    }
    for(int i = 100;i<200;i++){
        hedef[i] = -1;
    }

}

void egitim_test_ayristirma(double x[200][577],double x_egitim[160][577],double x_test[40][577]){
    int i,j;
    for(i=0; i<80; i++){
        for(j=0;j<577;j++){
            x_egitim[i][j] = x[i][j]; 
        }
        
    }
    for(i=100; i<180; i++){
        for(j=0;j<577;j++){
            x_egitim[i-20][j] = x[i][j]; 
        }
        
    }
    for(i=80; i<100; i++){
        for(j=0;j<577;j++){
            x_test[i-80][j] = x[i][j]; 
        }
        
    }
    for(i=180; i<200; i++){
        for(j=0;j<577;j++){
            x_test[i-160][j] = x[i][j]; 
        }
        
    }
}

void w_izleme_duzenleme(double w[577], double w_izleme[101][577], int iterasyon ){
    int i;
    for(i=0;i<577;i++){
        w_izleme[iterasyon][i] = w[i];
    }
}

void gradient_descent_w_duzenleme(double w[577],double x_egitim[160][577],double hedef[200] ){
    int i,j;
    double sum,eps=0.001 ;

    for(j=0;j<577;j++){

        sum = 0;

        for(i=0;i<160;i++){
            
            if(i<80){
                sum += loss_türevi(w[j],x_egitim[i][j],hedef[i]);
            }else{
                sum += loss_türevi(w[j],x_egitim[i][j],hedef[i+20]);
            }
            
        }

        w[j] = w[j] - eps*(sum/160) ;


        
    }

}

void gradient_descent(double x[200][577],double x_egitim[160][577],double x_test[40][577],double w[577],double w_izleme[101][577],int hedef[200]){
    int i,j,iterasyon;
    double sum;



    for(i=0;i<577;i++){
        w_izleme[0][577] = w[i];
    }

    for(iterasyon = 1;iterasyon <101;iterasyon++){
        gradient_descent_w_duzenleme(w,x_egitim,hedef);
        w_izleme_duzenleme(w,w_izleme,iterasyon);
    }
    
    
}

int main(){
    
    double x[200][577];
    double w[577];
    double x_egitim[160][577];
    double x_test[40][577];
    double w_izleme[101][577];
    int hedef[200];
    const char *filename = "data.csv";

    hedef_duzenleme(hedef);
    read_csv(filename, x);
    egitim_test_ayristirma(x,x_egitim,x_test);




    return 0;
}