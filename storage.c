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
	char passwd[PASSWD_LEN+1];
	
	char context[101];
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
	if (deliverySystem[x][y].cnt > 0)
		printf("<<<<<<<<<<<<<<<<<<<<<<<< : %s >>>>>>>>>>>>>>>>>>>>>>>>>>>>\n", deliverySystem[x][y].context);
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
	memset(&deliverySystem[x][y], 0, sizeof( storage_t));
	//??????? 일단해보고 스테이터스 출력해서 제대로 되고있는지 확인하기  
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
	//입력문자가 저장된 비밀번호나 마스터비밀번호와 같으면 꺼낼수있게함  
	if (strcmp(deliverySystem[x][y].passwd,inputPswrd) == 0||strcmp(masterPassword,inputPswrd))
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
	
	FILE *fp = fopen("storage.txt", "r");    // storage.txt 파일을 읽기 모드로 열기
	//근데 매크로가 안열린다...? 이름 똑같은데  
	
	//백업파일을 찾지 못했을 경우  
	if(fp == NULL)
		return -1;
	
	//백업파일을 정상적으로 찾았을 경우 
	//*택배보관함 생성*  
	fscanf(fp, "%d %d\n", &systemSize[0], &systemSize[1]);//택배보관함의 열과 행 수 받기   
	//받은 크기를 기반으로 택배보관함 생성  
	deliverySystem = ( storage_t**)calloc(systemSize[0],sizeof( storage_t*));
	
	if(deliverySystem == NULL)
	{
		printf("Not enough memory!\n");
		return -1;
	}
	
	for(i=0;i<systemSize[0];i++)
	{
		deliverySystem[i] = ( storage_t*)calloc(systemSize[1],sizeof(storage_t));
		
		printf("check line,  118 %d\n", &deliverySystem[3][5]);
		
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
	}//*/
	
	for(x=0;x<systemSize[0];x++)
	{
		for(y=0;y<systemSize[1];y++)
		{
		printf("for check, line 150 --- ");
		printf("%i %i <address> ", x, y);
		printf("%d %d <address reallife> \n", deliverySystem[x][y].building, deliverySystem[x][y].room);
		printf("%s <<<message>>>", deliverySystem[x][y].passwd); 
		printf("%s<<<message>>> \n\n", deliverySystem[x][y].context); 
		//주소설정을 잘못한 듯 하다 암튼 망했네  
		}
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
	//정상적인 입력인 경우 택배 보관 여부 반환 
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
	
	deliverySystem[x][y].building = nBuilding;
	deliverySystem[x][y].room = nRoom;
	*deliverySystem[x][y].context = msg[MAX_MSG_SIZE+1];//여기다시봐야함 대충 글쿤으로짰음  
	*deliverySystem[x][y].passwd = passwd[PASSWD_LEN+1];//여기다시봐야함 대충 글쿤으로짰음  
	
	msg[0] = '\0'; //메세지변수 초기화? 
	
	deliverySystem[x][y].cnt=1;
	storedCnt++;
	
	return 0; 
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {

	if(inputPasswd(x,y) == 0)
	{	//입력한 비번이 맞으면 내용을 출력
		printStorageInside(x,y); 
		
		//x,y의 저장되었던 정보 초기화   
		initStorage(x,y);
		
		//택배보관함이 다시 비었음을 표시  
		deliverySystem[x][y].cnt=0;
		storedCnt--;	
	}
	else
	{
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
	
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			if( deliverySystem[i][j].building == nBuilding&&deliverySystem[i][j].room == nRoom) 
			{	
				printf("-----------> Found a package in (%i, %i)", &i, &j);
				
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
	
	FILE *fp = fopen("STORAGE_FILEPATH", "w");
	
	if (fp == NULL) 
	{
	    printf("Backup error ! \n");
	    
	    return -1;
	}
	
	//백업파일을 정상적으로 열었을 경우 
	//*택배 행수, 열수* 
	fprintf(fp, "%d %d\n", &systemSize[0], &systemSize[1]);
	
	//*마스터키*
	fprintf(fp, "%s\n", &masterPassword[PASSWD_LEN+1]);
	
	//*저장된 택배 정보* 
	//입력된줄을 다출력하게하면되는디.....? 어카냐  
	
	for(i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		{
			if( deliverySystem[i][j].cnt == 1) 
			{	
				fprintf(fp, "%i %i", &i, &j);
				fprintf(fp, "%d %d", &deliverySystem[i][j].building, &deliverySystem[i][j].room);
				fprintf(fp, "%s %s\n", &deliverySystem[i][j].passwd, &deliverySystem[i][j].context);  
			}
		}
	}
	
	//파일닫기  
	fclose( fp);
	
	printf("<<Data backup.....");
	Sleep(500);//할리우드액숀  
	printf("completed>>\n");
	Sleep(500);//할리우드액숀
	
	return 0;
}
