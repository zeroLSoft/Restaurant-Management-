#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

#ifndef restaurant
#define restaurant
#define TABLES_NUM 15					// number of tables in the resturant
#define MAX 256							// maximun product's name

typedef struct Kitchen{					// Kitchen's product
	char* name;
	int quantity;
	float price;
	struct Kitchen* next;
}Kitchen, *PKitchen;

typedef struct Kitchen_List{			// List Manager for Kitchen's products.
	PKitchen head;	
	int types;							// number of product types
}Kitchen_List, *PKitchen_List;

typedef struct Table{					// a node for table product
	int HappyFlag, glutenFlag;			// happy moment and gluten flags. glutenFlag = 0 ==> gluten free.
	char* name;
	float price;
	int quantity;
	struct Table* next, *prev;
}Table, *PTable;

typedef struct Room{					// struct for dining room info.
	struct Table* Tables[TABLES_NUM];	// array of tables.
	int occupied;						// tables in use.
	struct Table* Trash;					// cancelled portions.
}Dining_Room, *PDining_Room;

/*Primary functions*/
int CreateProduct(PKitchen_List list);	// get a kitchen list, add a product. return 1 if memory allocation failed.
void AddItems(PKitchen_List list);		// get a kitchen list, add supplies to a product.
int OrderItem(int TableNumber, char* ProductName, int glutenFlag, PKitchen_List list, PDining_Room dining); /*get table data, product data, kitchen list and
 add a product to a table. return 1 if memory allocation failed.*/
int RemoveItem(PDining_Room dining);	// get tables data, remove a product from a table.
void HappyMoments(PDining_Room dining, PKitchen_List list, PKitchen_List glutenFree);	/*get tables data, turn on flag 
for the most ordered product.*/
void ChangeTable(PDining_Room dining);	// get tables data, transfer a table to highest numbered empty table.
void RemoveTable(PDining_Room dining);	// get tables data, check out a table and free all its nodes.

/*Secondary functions*/
int preCreateProduct(PKitchen_List list, PKitchen_List glutenFree);	/* get both lists, choose one to add a product to.*/
void preAddItems(PKitchen_List list, PKitchen_List glutenfree);		/* get both lists, choose one to add supplies to.*/
void preOrderItem(PKitchen_List list, PKitchen_List glutenFree, PDining_Room dining); /*get both kitchen lists and tables, 
choose a list to make order from*/
int Count_Portions(PDining_Room dining, PKitchen_List list, int glutenFlag, int *CountArr);		/*get a kitchen list and tables data, returns
the quantity of max orders of a product in that list.*/
void Turn_HappyON(PDining_Room dining, PKitchen_List list, int* CountArr, int max, int glutenFlag);	// turn on happy flag for the max orders.
/*Even more secondary functions*/
void initialize_Kitchen(PKitchen_List list);		// initialize a kitchen product list.
void Free_Kitchen(PKitchen_List list);				// free list made of kitchen products.
int Search_Kitchen(PKitchen_List list, char* str);	// search for a product in a kitchen list. returns 1 if found.
void initializeTables(PDining_Room dining);			//  initialize tables in the dining room.
void Free_Tables(PDining_Room dining);				// free allocated memory for ordered food in the tables.
int Check_TableNum(int num);						// check if table number is valid, returns 1 if NOT!
int Check_Sign(void* num, char *str);				// get number description of that number. returns 1 if negative.
void Print_Menu(PKitchen_List list);				// get a kitchen list and print it as a menu.
void Print_List(PKitchen_List list);				// get a kitchen list an print it as a linked list.
void Error_Msg(char *str);							// print msg and exit program with error.
void Epic_Fail(PDining_Room dining, PKitchen_List list, PKitchen_List glutenFree);		/* free all the allocated memory 
by the program and exit with error.*/
void Print_Status(PDining_Room dining, PKitchen_List list, PKitchen_List glutenFree);	// print all restaurant stocks and tables status.


#endif // !Resturant
