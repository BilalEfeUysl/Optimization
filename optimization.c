#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define N 28 
#define EPS 0.001
#define LINE_LENGTH 1024 

void read_csv(const char *filename, double **matrix);
void generate_w(double *w,double baslangic);
double derivative_loss(double w,double x,int y);
void y_edit(int *y);
void separation_train_Test(double **x,double **x_train,double **x_test);
void w_history_edit(double *w, double **w_history, int iteration );
void gradient_descent_w_edit(double *w,double **x_train,int *y );
void gradient_descent(double **x,double **x_train,double **x_test,double *w,double **w_history,int *y,double loss_history[101]);
void Adam_Algorithm(double **x,double *w,double **x_train,double **x_test,double **w_history,int *y,double loss_history[101]);
void stochastic_gradient(double **x,double **x_train,double **x_test, double *w, double **w_history,int *y,double loss_history[101]);

void read_csv(const char *filename, double **matrix) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Dosya acilamadi: %s\n", filename);
        return;
    }

    char line[LINE_LENGTH];
    int row = 0;

    // Dosyayı satır satır oku
    while (fgets(line, sizeof(line), file) && row < 200) {
        int col = 0;
        char *token = strtok(line, ";");

        // Satırdaki her sütunu virgülle ayırarak okuma
        while (token && col < 785) {
            matrix[row][col] = atof(token);  // Veriyi sayıya çevirerek matrise ekle
            token = strtok(NULL, ";");
            col++;
        }
        row++;
    }

    fclose(file);
}

void generate_w(double *w,double baslangic){
	int i;
    for(i=0;i<785;i++){
        w[i] = baslangic;
    }

}

double derivative_loss(double w,double x,int y){

    return -2*(y-tanh(w*x))*(1-pow(tanh(w*x),2))*x;


}

void y_edit(int *y){
	int i;
    for(i=0;i<100;i++){
        y[i] = 1;
    }
    for(i = 100;i<200;i++){
        y[i] = -1;
    }

}

void separation_train_Test(double **x,double **x_train,double **x_test){
    int i,j;
    for(i=0; i<80; i++){
        for(j=0;j<785;j++){
            x_train[i][j] = x[i][j]; 
        }
        
    }
    for(i=100; i<180; i++){
        for(j=0;j<785;j++){
            x_train[i-20][j] = x[i][j]; 
        }
        
    }
    for(i=80; i<100; i++){
        for(j=0;j<785;j++){
            x_test[i-80][j] = x[i][j]; 
        }
        
    }
    for(i=180; i<200; i++){
        for(j=0;j<785;j++){
            x_test[i-160][j] = x[i][j]; 
        }
        
    }

}

void w_history_edit(double *w, double **w_history, int iteration ){
    int i;
    for(i=0;i<785;i++){
        w_history[iteration][i] = w[i];
    }
}

void gradient_descent_w_edit(double *w,double **x_train,int *y ){
    int i,j;
    double sum;
    

    for(j=0;j<785;j++){

        sum = 0;

        for(i=0;i<160;i++){
            
            if(i<80){
                sum += derivative_loss(w[j],x_train[i][j],y[i]);
            }else{
                sum += derivative_loss(w[j],x_train[i][j],y[i+20]);
            }
            
        }

        w[j] = w[j] - EPS*(sum/160.0) ;


        
    }

}
void stochastic_gradiant_w_edit(double *w,double **x_train,int *y){
    int i,j;
    double derivative;

    i = rand() % 160;

    for(j=0;j<785;j++){
        if(i<80){
            derivative = derivative_loss(w[j],x_train[i][j],y[i]);   
        }else{
            derivative = derivative_loss(w[j],x_train[i][j],y[i+20]);
        }
        w[j] = w[j] - EPS*(derivative);
    }


}

double calculate_loss(double *w,double **x_train,int *y){
    double sum, sum_loss = 0;
    int i,j;

    for(i=0;i<160;i++){
        sum = 0;
        for(j=0;j<785;j++){
            sum += w[j]*x_train[i][j];
        }

        if(i<80){
            sum_loss += pow((y[i]-tanh(sum)),2);
        }else{
            sum_loss += pow((y[i+20]-tanh(sum)),2);
        }
    }

    return (sum_loss / 160.0);


}

void gradient_descent(double **x,double **x_train,double **x_test,double *w,double **w_history,int *y,double loss_history[101]){
    int i,j,iteration;
    double sum,accuracy_rate;
    int result[40];



    for(i=0;i<785;i++){
        w_history[0][785] = w[i];
    }

    loss_history[0] = calculate_loss(w,x_train,y); 

    for(iteration = 1;iteration <101;iteration++){

        gradient_descent_w_edit(w,x_train,y);
        w_history_edit(w,w_history,iteration);

        loss_history[iteration] = calculate_loss(w,x_train,y);
    }
    
    for(i=0;i<40;i++){
        sum =0;
        for(j=0;j<785;j++){
            sum += x_test[i][j]*w[j]; 
        }

        if(i<20){
            
            if(tanh(sum) >= 0.5){
                result[i] = 1;
                printf("Doğru buldi\n");
            }else{
                result[i] = 0;
            }


        }else{
            if(tanh(sum) <= -0.5){
                result[i] = 1;
                printf("Doğru buldi\n");
            }else{
                result[i] = 0;
            }
        }
    }

    sum = 0;

    for(i<0;i<40;i++){
        sum += result[i];
    }

    accuracy_rate = sum/40.0;
    
}

void stochastic_gradient(double **x,double **x_train,double **x_test, double *w, double **w_history,int *y,double loss_history[101]){
    int i,j,iteration,result[40];
    double sum,accuracy_rate;

    for(i=0;i<785;i++){
        w_history[0][785] = w[i];
    }
    for(iteration = 1;iteration <101;iteration++){
        stochastic_gradiant_w_edit(w,x_train,y);
        w_history_edit(w,w_history,iteration);

        loss_history[iteration] = calculate_loss(w,x_train,y);
    }
    for(i=0;i<40;i++){
        sum =0;
        for(j=0;j<785;j++){
            sum += x_test[i][j]*w[j]; 
        }

        if(i<20){
            
            if(tanh(sum) >= 0.5){
                result[i] = 1;
            }else{
                result[i] = 0;
            }


        }else{
            if(tanh(sum) <= -0.5){
                result[i] = 1;
            }else{
                result[i] = 0;
            }
        }
    }

    
    sum = 0;
    for(i<0;i<40;i++){
        sum += result[i];
    }

    accuracy_rate = sum/40.0;


}


void Adam_Algorithm(double **x,double *w,double **x_train,double **x_test,double **w_history,int *y,double loss_history[101]){
    double learningRate = 0.001,b1 = 0.9,b2 = 0.999,e = pow(10,-8),gt,mt=0,vt=0,sum,accuracy_rate;
    int i,j,iteration,result[40];

    for(i=0;i<785;i++){
        w_history[0][785] = w[i];
    }
    loss_history[0] = calculate_loss(w,x_train,y);

    for(iteration = 1;iteration<101;iteration++){
        j = rand() % 160;

        for(i=0;i<785;i++){
            if(j<80){
                gt = derivative_loss(w[i],x_train[j][i],y[j]);
            }else{
                gt = derivative_loss(w[i],x_train[j][i],y[j+20]);
            }

            mt = (b1 * mt) + (1.0 - b1)*gt;
            vt = (b2 * vt) + (1.0 - b2)*pow(gt,2);

            w[i] -= (learningRate*mt)/sqrt(vt + e);
            
        }

        w_history_edit(w,w_history,iteration);

        loss_history[iteration] = calculate_loss(w,x_train,y);


    }

    for(i=0;i<40;i++){
        sum =0;
        for(j=0;j<785;j++){
            sum += x_test[i][j]*w[j]; 
        }

        if(i<20){
            
            if(tanh(sum) >= 0.5){
                result[i] = 1;
            }else{
                result[i] = 0;
            }


        }else{
            if(tanh(sum) <= -0.5){
                result[i] = 1;
            }else{
                result[i] = 0;
            }
        }
    }

    sum = 0;

    for(i<0;i<40;i++){
        sum += result[i];
    }

    accuracy_rate = sum/40.0;

    
}

void edit_x(double **x) {
    for (int i = 0; i < 200; i++) {
        for (int j = 0; j < 785; j++) {
            x[i][j] = x[i][j]/255.0;
        }
    }
}

int main(){

    srand(time(0));
    int i;
    double loss_history[101];

    double **x = (double **)calloc(200,sizeof(double *));
    for(i=0;i<200;i++){
        x[i] = (double *)calloc(785,sizeof(double));
    }
    double *w = calloc(785,sizeof(double));
    double **x_train;
    x_train = (double **)calloc(160,sizeof(double *));
    for(i=0;i<160;i++){
        x_train[i] = (double *)calloc(785,sizeof(double));
    }
    double **x_test;
    x_test = (double **)calloc(40,sizeof(double *));
    for(i=0;i<40;i++){
        x_test[i] = (double *)calloc(785,sizeof(double));
    }
    double **w_history;
    w_history = (double **)calloc(101,sizeof(double *));
    for(i=0;i<101;i++){
        w_history[i] = (double *)calloc(785,sizeof(double));
    }
    int *y = calloc(200,sizeof(int));
    const char *filename = "data.csv";


    read_csv(filename,x);
    edit_x(x);

    generate_w(w,0.1);
    y_edit(y);
    
    separation_train_Test(x,x_train,x_test);

    //gradient_descent(x,x_train,x_test,w,w_history,y,loss_history);
    //stochastic_gradient(x,x_train,x_test,w,w_history,y,loss_history);
    //Adam_Algorithm(x,w,x_train,x_test,w_history,y,loss_history);
    
    /*for (int j = 0; j < 101; j++) {
            printf("%.10f \n", loss_history[j]);  
        }
    */

    printf("calisiyo");
    return 0;
}
