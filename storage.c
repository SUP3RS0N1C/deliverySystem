#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define STORAGE_FILEPATH 		"storage.txt"
#define MAX_MSG_SIZE			100
#define PASSWD_LEN				4

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/
typedef struct {
	int building;//동 수 (1~10)  
	int room;//호수 ex)308  
	int cnt;//택배가 들었는지 여부  
	char passwd[PASSWD_LEN+1];//비밀번호 저장  
	
	char *context;//메세지 저장 
} storage_t;


static storage_t** deliverySystem; 			//deliverySystem
static int storedCnt = 0;					//number of cells occupied
static int systemSize[2] = {0, 0};  		//row/column of the delivery system
static char masterPassword[PASSWD_LEN+1];	//master password




// ------- inner functions ---------------

//print the inside context of a specific cell
//int x, int y : cell to print the context
static void printStorageInside(int x, int y) {
	printf("\n------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n");
	if (deliverySystem[x][y].cnt > 0)//택배가 들어있다면  
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", &deliverySystem[x][y].context);//내용을 출력  
	else
		printf("<<<<<<<<<<<<<<<<<<<<<<<< empty >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
		
	printf("------------------------------------------------------------------------\n");
	printf("------------------------------------------------------------------------\n\n");
}

//initialize the storage
//set all the member variable as an initial value
//and allocate memory to the context pointer
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	memset(&deliverySystem[x][y], 0, sizeof( storage_t));//(x,y)의 structure 초기화  
	
	return;  
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	char inputPswrd[PASSWD_LEN+1];
	
	//비밀번호 입력받기  
	printf(" - input password for (%d, %d) storage : ", x,y);
	scanf("%s",inputPswrd);
	
	//check password
	//입력문자가 저장된 비밀번호나 마스터비밀번호와 같으면 0반환 아닐시 -1 빈환  
	if (strcmp(deliverySystem[x][y].passwd,inputPswrd) == 0||strcmp(masterPassword,inputPswrd) == 0)
		return 0; 
	else 
		return -1;
}



// ------- API function for main.c file ---------------

//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system)
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {

	int i; 
	int x,y;
	int inProceed;
	
	FILE *fp = fopen(STORAGE_FILEPATH, "r");    // storage.txt 파일을 읽기 모드로 열기 
	
	//백업파일을 찾지 못했을 경우  
	if(fp == NULL)
		return -1;
	
	//백업파일을 정상적으로 찾았을 경우 
	//*택배보관함 생성*  
	fscanf(fp, "%d %d\n", &systemSize[0], &systemSize[1]);//택배보관함의 열과 행 수 받기   
	//받은 크기를 기반으로 택배보관함 생성  
	deliverySystem = ( storage_t**)calloc(systemSize[0],sizeof( storage_t*));//열 생성  
	//(메모리가 부족해) 생성이 되지 않은 경우  
	if(deliverySystem == NULL)
	{
		printf("Not enough memory!\n");
		return -1;
	}
	
	for(i=0;i<systemSize[0];i++)
	{
		deliverySystem[i] = ( storage_t*)calloc(systemSize[1],sizeof(storage_t));//행 생성  
		//(메모리가 부족해) 생성이 되지 않은 경우  
		if(deliverySystem == NULL){
			printf("Not enough memory!\n");
			return -1;
		}
	}
	
	
	//*마스터키 받기*
	fscanf(fp, "%s", masterPassword);
	
	//*초기 저장된 택배 정보 받기* 
	storedCnt = 0;
	
	while(1)//저장되어있던 데이터를 반복해서 불러옴  
	{	
		fscanf(fp, "%i %i", &x, &y);
		fscanf(fp, "%d %d", &deliverySystem[x][y].building, &deliverySystem[x][y].room);
		fscanf(fp, "%s ", &deliverySystem[x][y].passwd); 
		fscanf(fp, "%s ", &deliverySystem[x][y].context); 
				
		deliverySystem[x][y].cnt = 1;//택배 저장됨을 표시  
		storedCnt++;//택배갯수 
		
		if(feof(fp))break; 
	}
	
	//파일닫기  
	if(fclose(fp) != 0)
	
	return 0; 
}


//free the memory of the deliverySystem 
void str_freeSystem(void) {
	int i;
	static int systemSize[0];
	
	//메모리풀기  
	for (i=0; i<systemSize[0]; i++)
        free(deliverySystem[i]);
    free(deliverySystem);
}



//print the current state of the whole delivery system (which cells are occupied and the destination of the each occupied cells)
void str_printStorageStatus(void) {
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt, systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf("  -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}



//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	//입력한 열이 음수일 경우 & 택배보관함의 사이즈보다 큰 경우
	if (x < 0 || x >= systemSize[0])  
	{
		return -1;
		//Storage is already occupied or invalid가 출력되도록 함  
	}
	//입력한 행이 음수일 경우 & 택배보관함의 사이즈보다 큰 경우
	if (y < 0 || y >= systemSize[1])
	{
		return -1;
		//Storage is already occupied or invalid가 출력되도록 함
	}
	//정상적인 입력인 경우 택배가 차있는지 여부 반환 
	return deliverySystem[x][y].cnt;	 
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	//입력받은 내용을 (x,y)에 해당하는 structure에 저장  
	deliverySystem[x][y].building = nBuilding;
	deliverySystem[x][y].room = nRoom;
	strcpy(deliverySystem[x][y].passwd, passwd);
	deliverySystem[x][y].context = (char*)malloc(strlen(msg) + 1);//여기랑  
	strcpy(deliverySystem[x][y].context, msg);//여기가문제인거같음  checkline
	//택배가 저장되었음을 표시  
	deliverySystem[x][y].cnt=1;
	//총 저장된 택배 수를 하나 더함  
	storedCnt++;
	
	return 0; 
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	//입력한 비밀번호가 맞으면
	if(inputPasswd(x,y) == 0)
	{	//내용을 출력
		printStorageInside(x,y); 
		
		//(x,y)에 저장되었던 정보 초기화   
		initStorage(x,y);
		//(x,y)에 저장된 텍스트를 저장하기 위해 할당했던 동적메모리 초기화  
		free(deliverySystem[x][y].context);
		
		//해당 택배보관함이 다시 비었음을 표시  
		deliverySystem[x][y].cnt=0;
		//총택배갯수 감소  
		storedCnt--;	
	}
	//비밀번호가 틀렸을 경우
	else
	{	//틀렸음을 출력  
		printf(" -----------> password is wrong!!\n");
		
		return -1; 
	}
	
	return 0; 
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	int cnt = 0;
	int i,j;
	//조건에 만족하는 (x,y)를 찾기 위해 존재하는 택배보관함을 모두 살펴보기  
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{	//입력한 동수와 호수가 모두 일치하는 (x,y)가 있으면 택배가 존재한다는것을 출력  
			if( deliverySystem[i][j].building == nBuilding&&deliverySystem[i][j].room == nRoom) 
			{	//택배가 있음을 출력  
				printf("-----------> Found a package in (%d, %d)\n", i, j);
				//입력한 동수와 호수가 찾을 수 있는 택배 개수를 카운트  
				cnt++;
			}
		}
	} 
	
	return cnt;
}



//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	int i,j;
	//백업파일열기  
	FILE *fp = fopen(STORAGE_FILEPATH, "w+");
	//백업파일을 여는데 실패했을 경우 방어코드  
	if (fp == NULL) 
	{
	    printf("Backup error ! \n");
	    
	    return -1;
	}
	
	//백업파일을 정상적으로 열었을 경우 
	//*택배 행수, 열수* 
	fprintf(fp, "%d %d\n", systemSize[0], systemSize[1]);
	
	//*마스터키*
	fprintf(fp, "%s\n", masterPassword);
	
	//*저장된 택배 정보* 
	//택배가 저장되어 있는 칸은 텍스트로 내용이 출력되도록 함  
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{	//택배가 들어있을 경우  
			if( deliverySystem[i][j].cnt == 1) 
			{	//저장된 내용 텍스트로 내보내기  
				fprintf(fp, "%i %i ", i, j);
				fprintf(fp, "%d %d ", deliverySystem[i][j].building, deliverySystem[i][j].room);
				fprintf(fp, "%s %s\n", &deliverySystem[i][j].passwd, &deliverySystem[i][j].context);  
			}
		}
	}
	
	//파일닫기  
	fclose( fp);
	//파일이 백업되었음을 표시  
	printf("\n<<Data backup");
	Sleep(100);//할리우드액숀 
	printf(".");
	Sleep(100);//할리우드액숀 
	printf(".");
	Sleep(100);//할리우드액숀 
	printf(".");
	Sleep(100);//할리우드액숀 
	printf(".");
	Sleep(300);//할리우드액숀 
	printf("completed>>\n");
	
	return 0;
}
