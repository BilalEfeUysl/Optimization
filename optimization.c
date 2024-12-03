#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define N 24 // Görüntü boyutu NxN

void read_csv(const char *filename, double **matrix) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Dosya acilamadi");
        exit(EXIT_FAILURE);
    }
	int i,j;
    for (i = 0; i < 200; i++) {
        for (j = 0; j < 577; j++) {
            if (fscanf(file, "%lf,", &matrix[i][j]) != 1) {
                fprintf(stderr, "Dosya formati hatali veya beklenmeyen veri\n");
                fclose(file);
                exit(EXIT_FAILURE);
            }
        }
    }

    fclose(file);
}

void generate_w(double *w,double baslangic){
	int i;
    for(i=0;i<577;i++){
        w[i] = baslangic;
    }

}

double loss_turevi(double w,double x,int hedef){

    return -2*(hedef-tanh(w*x))*(1-pow(tanh(w*x),2))*x;


}

void hedef_duzenleme(int *hedef){
	int i;
    for(i=0;i<100;i++){
        hedef[i] = 1;
    }
    for(i = 100;i<200;i++){
        hedef[i] = -1;
    }

}

void egitim_test_ayristirma(double **x,double **x_egitim,double **x_test){
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

    printf("buraya kadar calisti mi");
}

void w_izleme_duzenleme(double *w, double **w_izleme, int iterasyon ){
    int i;
    for(i=0;i<577;i++){
        w_izleme[iterasyon][i] = w[i];
    }
}

void gradient_descent_w_duzenleme(double *w,double **x_egitim,int *hedef ){
    int i,j;
    double sum,eps=0.001 ;

    for(j=0;j<577;j++){

        sum = 0;

        for(i=0;i<160;i++){
            
            if(i<80){
                sum += loss_turevi(w[j],x_egitim[i][j],hedef[i]);
            }else{
                sum += loss_turevi(w[j],x_egitim[i][j],hedef[i+20]);
            }
            
        }

        w[j] = w[j] - eps*(sum/160.0) ;


        
    }

}

void gradient_descent(double **x,double **x_egitim,double **x_test,double *w,double **w_izleme,int *hedef){
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

    int i;
    double **x = (double **)malloc(200*sizeof(double *));
    for(i=0;i<200;i++){
        x[i] = (double *)malloc(577*sizeof(double));
    }
    double *w = malloc(577*sizeof(double));
    double **x_egitim;
    x_egitim = (double **)malloc(160*sizeof(double *));
    for(i=0;i<160;i++){
        x_egitim[i] = (double *)malloc(577*sizeof(double));
    }
    double **x_test;
    x_test = (double **)malloc(40*sizeof(double *));
    for(i=0;i<40;i++){
        x_test[i] = (double *)malloc(577*sizeof(double));
    }
    double **w_izleme;
    w_izleme = (double **)malloc(101*sizeof(double *));
    for(i=0;i<40;i++){
        w_izleme[i] = (double *)malloc(577*sizeof(double));
    }
    int *hedef = malloc(200*sizeof(int));


    //const char *filename = "data.csv";

    hedef_duzenleme(hedef);
    // read_csv(filename, x);
    egitim_test_ayristirma(x,x_egitim,x_test);


    printf("Kod calisiyor");

    return 0;
}
