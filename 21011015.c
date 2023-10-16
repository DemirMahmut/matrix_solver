#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h> // Renk eklemek için gereken kütüphane
#define MAX 15
#define MAXSIZE 10

void setConsoleColor(int colorCode);  // Renk ayarlamak için fonksiyon
void drawBoard(int matris[][MAX], int N);
void readFromFile(int matris[][MAX], char *fileName);
void manualSolve(int matris[][MAX], int N);
void generateRandomMatrix(int matris[][MAX],int N);
void copyMatrix(int matris[][MAX], int tempMatrix[][MAX], int N);
void undoMove(int matris[][MAX], int tempMatrix[][MAX], int N);
void autoSolve(int matris[][MAX], int N);

int main(){
    int N, i, j, choice, data, next; 
    int  ctrl = 1, flag = 1, again = 1, undo = 0, matris[MAX][MAX]={{0}}, tempMatrix[MAX][MAX]={{0}};
	char quit, fileName[20];
	printf("--- ANA MENU ---\n0- Oyunu bitir.\n1- Dosyadan hazir matris al.\n2- Matrisi manuel olustur.\n3- Diger oyuncularin skorlarini gor.\nTercihiniz: ");
	scanf("%d", &choice);
	while(flag == 1){
		if(choice == 0){
			flag = 0;
		}
		else if(choice == 1){	
  			printf("Dosya adini giriniz: ");
  			scanf("%s",fileName);
  			readFromFile(matris, fileName);
	  		printf("Oyun alaninin boyutunu giriniz: ");
			scanf("%d", &N);
	    	drawBoard(matris,N);
	    	printf("\n1- Manuel cozum\n2- Otomatik cozum\nYapmak istediginiz cozum islemini seciniz: ");
	    	scanf("%d", &choice);
	    	if(choice == 1){
	    		do{
	    			copyMatrix(matris, tempMatrix, N);
					manualSolve(matris, N);
					drawBoard(matris, N);
					printf("\nCikmak icin [0]\nHamleyi geri almak icin [1]\nDevam etmek icin [2] tuslayiniz.\n");
					scanf("%d", &next);
					if(next == 1){
						undoMove(matris, tempMatrix, N);
						drawBoard(matris, N);
					}
				}while(next != 0);
				for (i = 0; i < N; i++){
        				for (j = 0; j < N; j++){
            				if (matris[i][j] == 0){
								ctrl = 0;
						}
					}
				}
				if(ctrl == 0)
					printf("Matriste bosluk var. Kaybettiniz!");
				flag = 0;
			}
			else{
				autoSolve(matris, N);
				drawBoard(matris, N);
				flag = 0;
			}
			
		}
	
		else if(choice == 2){
			printf("Olusturmak istediginiz matrisin boyutunu giriniz: ");
			scanf("%d", &N);
			do{
				generateRandomMatrix(matris, N);
				drawBoard(matris, N);
				printf("\n1- Matrisi coz.\n2- Yeni matris iste.\nTercihiniz: ");
				scanf("%d", &again);
			}while(again == 2);
			printf("\n1- Manuel cozum\n2- Otomatik cozum\nYapmak istediginiz cozum islemini seciniz: ");
	    	scanf("%d", &choice);
	    	if(choice == 1){
	    		do{
	    			copyMatrix(matris, tempMatrix, N);
					manualSolve(matris, N);
					drawBoard(matris, N);
					for (i = 0; i < N; i++){
        				for (j = 0; j < N; j++){
            				if (matris[i][j] == 0){
								next = 0;
								printf("Matriste bosluk var. Kaybettiniz!");
							}
						}
					}
						printf("\nCikmak icin [0]\nHamleyi geri almak icin [1]\nDevam etmek icin [2] tuslayiniz.\n");
						scanf("%d", &next);
					if(next == 1){
						undoMove(matris, tempMatrix, N);
						drawBoard(matris, N);
					}
				}while(next != 0);
				for (i = 0; i < N; i++){
        				for (j = 0; j < N; j++){
            				if (matris[i][j] == 0){
								ctrl = 0;								
						}
					}
				}
				if(ctrl == 0)
					printf("Matriste bosluk var. Kaybettiniz!");
				flag = 0;
			}
			else{
				autoSolve(matris, N);
				drawBoard(matris, N);
				flag = 0;
			}
	}
	printf("\nCikmak icin bir tus giriniz: ");
	scanf(" %c", &quit);
	return 0;
	}
}
void readFromFile(int matris[][MAX], char *fileName){
	int i, j, temp;
	FILE *data = fopen(fileName,"r");
	if(!data){
        printf("\nDosya Acilamadi! Dosya adini tekrar giriniz: ");
        scanf("%s", fileName);
        readFromFile(matris, fileName);
        return;
    }
    
    while(!feof(data)){
        fscanf(data,"%d %d %d\n",&i,&j,&temp);  
		matris[i][j]=temp;
    }  
  	fclose(data); 
}
void setConsoleColor(int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colorCode);
}
void drawBoard(int matris[][MAX], int N){
    int i,j,k;
    printf("\n   ");
    for(i=0;i<N;i++){
    	if(i>9){
    		printf("   %d  ", i);
    	}else{
    		printf("   %d   ", i);
    	}
    }
    for(i=0;i<N;i++){
        printf("\n   -");
        for (k=0;k<N;k++)
            printf("-------");
        printf("\n%2d |",i);
        for(j=0;j<N;j++)
            if (matris[i][j]!=0){
            	setConsoleColor(matris[i][j]);  // Renk ayarý
            	if(matris[i][j]>9)
                	printf("  %d  ",matris[i][j]);
                else
                	printf("  %d   ",matris[i][j]);
                setConsoleColor(7);  // Varsayýlan renk
                printf("|");
            }
            else
                printf("      |",matris[i][j]);
    }
    printf("\n   -");
    for (k=0;k<N;k++){
            printf("-------");
    }
    printf("\n");
}
void manualSolve(int matris[][MAX], int N){
    int row, col, direction, number, i, target, flag = 1;
    printf("Baslamak istediginiz noktayi giriniz (Satir Sutun): ");
	scanf("%d %d", &row, &col);
    while(flag == 1){
    	if(row >= 0 && row < N && col >= 0 && col < N && matris[row][col] != 0){
			flag = 0;
		}
		else{
			printf("Gecersiz baslangic noktasi! Tekrar giriniz: ");
			scanf("%d %d", &row, &col);
		}
	}
	target = matris[row][col];
	printf("Sectiginiz nokta: %d", target);
    do{
	printf("\nHareket yonunu giriniz (1: Sag, 2: Sol, 3: Yukari, 4: Asagi): \n");
    scanf("%d", &direction);
	}while(direction > 4  || direction < 1);
    printf("Belirtilen yonde kac birim gitmek istediginizi giriniz: \n");
    scanf("%d", &number);
    flag = 1;
    while(flag == 1){
    	if(direction == 1){
    		i = col+1;
	    	while(i < col+number+1 && matris[row][i] == 0 && i < N){
	    		matris[row][i] = target;
				i++;
			}
			flag = 0;
		}
		else if(direction == 2){
			i = col-1;
	    	while(i > col-number-1 && matris[row][i] == 0 && i >= 0){
	    		matris[row][i] = target;
				i--;
			}
			flag = 0;
		}
		else if(direction == 3){
			i = row-1; 
	    	while(i > row-number-1 && matris[i][col] == 0 && i >= 0){
	    		matris[i][col] = target;
	    		i--;
			}
			flag = 0;
		}
		else if(direction == 4){
			i = row+1; 
	    	while(i < row+number+1 && matris[i][col] == 0 && i < N){
	    		matris[i][col] = target;
	    		i++;
			}
			flag = 0;
		}
	}
    return;
}
    	/*for(i = col; i < col+number; i++){
    		while(matris[row][i] != 0){
    			printf("Hatali giris! Tekrar deneyiniz.\n");
    			printf("Hareket yonunu giriniz (1: Sag, 2: Sol, 3: Yukari, 4: Asagi): \n");
			    scanf("%d", &direction);
			    printf("Belirtilen yonde kac birim gitmek istediginizi giriniz: \n");
			    scanf("%d", &number);*/
void generateRandomMatrix(int matris[][MAX],int N){
    int i, j, Row, Col;
    int num = 1;

    // Rastgele sayý üreteci için zamaný kullanýr
    srand(time(NULL));

    // Matrisi boþ olarak baþlatýr
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matris[i][j] = 0;
        }
    }

    // Matrisin içine N'e kadar olan sayýlarý 2'þer tane yerleþtirir
    while(num <= N) {
    	j = 0;
    	while(j < 2){
    		Row = rand() % N+0;
        	Col = rand() % N+0;
    		if(matris[Row][Col] == 0){
    			matris[Row][Col] = num;
    			j++;
			}
		}
		num++;
    }
    return;
}
void copyMatrix(int matris[][MAX], int tempMatrix[][MAX], int N){
	int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            tempMatrix[i][j] = matris[i][j];
        }
    }
}
// Hamleyi geri alma fonksiyonu
void undoMove(int matris[][MAX], int tempMatrix[][MAX], int N){
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matris[i][j] = tempMatrix[i][j];
        }
    }
}
void autoSolve(int matris[][MAX], int N){
    int solved[MAX][MAX] = {0};
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (matris[i][j] != 0) {
                // Üst taraf
                if (i > 0 && matris[i - 1][j] == 0)
                    solved[i - 1][j] = matris[i][j];

                // Alt taraf
                if (i < N - 1 && matris[i + 1][j] == 0)
                    solved[i + 1][j] = matris[i][j];

                // Sol taraf
                if (j > 0 && matris[i][j - 1] == 0)
                    solved[i][j - 1] = matris[i][j];

                // Sað taraf
                if (j < N - 1 && matris[i][j + 1] == 0)
                    solved[i][j + 1] = matris[i][j];
                
                // Kendi hücre
                solved[i][j] = matris[i][j];
            }
        }
    }
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            matris[i][j] = solved[i][j];
        }
    }
}
