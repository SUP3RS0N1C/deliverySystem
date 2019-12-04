#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

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
	int building;//�� �� (1~10) 
	int room;//ȣ�� ex)308  
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
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
	
}

//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
}





// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) {
	
}



//create delivery system on the double pointer deliverySystem
//char* filepath : filepath and name to read config parameters (row, column, master password, past contexts of the delivery system)
//return : 0 - successfully created, -1 - failed to create the system
int str_createSystem(char* filepath) {
	
	FILE *fp;
	static int ROW,COLUMN; //������ �ù躸���� ũ��  
	static int deliverySystem[ROW][COLUMN] = { 0, };//�ù躸���� ����  
													// �ù躸���� �ʱ�ȭ  
	int i; 
	int x,y;
	
	fp = fopen("STORAGE_FILEPATH", "r+");    // storage.txt ������ �б� ���� ����
	//��������� ã�� ������ ���  
	if(fp == NULL)
		return 0;
	
	//��������� ���������� ã���� ���  
	fscanf(fp, "%d %d", &ROW, &COLUMN); //�ù躸������ ���� �� �� �ޱ�  
	
	fscanf(fp, "%s", &masterPassword[PASSWD_LEN+1]);//������Ű �ޱ�
	
	while( EOF != fscanf(fp, "%i %i %d %d %s %s", &x, &y, &deliverySystem[x][y].building, &deliverySystem[x][y].room, deliverySystem[x][y].passwd, &deliverySystem[x][y].context))//����Ǿ��ִ� �����͸� �ݺ��ؼ� �ҷ���  
	{
		fscanf(fp, "%i %i %d %d %s %s", &x, &y, &deliverySystem[x][y].building, &deliverySystem[x][y].room, deliverySystem[x][y].passwd, &deliverySystem[x][y].context);//�ʱ� ����� �ù� ���� �ޱ�  
	}
	
	fclose( fp);
	
	return 1; 
}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
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
	if (x < 0 || x >= systemSize[0])//�Է��� ���� ������ ��� & �ù躸������ ������� ū ���  
	{
		return -1;//Storage is already occupied or invalid�� ��µǵ��� ��  
	}
	
	if (y < 0 || y >= systemSize[1])//�Է��� ���� ������ ��� & �ù躸������ ������� ū ���
	{
		return -1;//Storage is already occupied or invalid�� ��µǵ��� ��
	}
	
	return deliverySystem[x][y].cnt;	//�������� �Է��� ��� �ù� ���� ���� ��ȯ  
}


//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	
	//**************************************************************************************************	
	//**************************************************************************************************
	���� ������ ����!!!!!!!!!!!! ���ο��� ���Լ�ã������ �ű���� �ٽ� �о������ ��  
	//**************************************************************************************************
	//**************************************************************************************************
	
	//������ deliverySystem[x][y].cnt=1���� �ù��������� �˷�����  
	return 0; 
}



//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	//��й�ȣ Ȯ��  
	printf(" - input password for (%d, %d) storage : ", x,y);
	//scanf�� ����Է¹ޱ�?  
	
	if()//�Է��� ����� ������ ������ ���
	{
		//printStorageInside(x,y); 
		//deliverySystem[x][y].cnt=0 �ؼ� ����ȵǾ������� �˷����� 
	}
	
	return 0; 
}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	return cnt;
}