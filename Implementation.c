#include "Header.h"

/*Primary functions*/

/* get dining room tables, and remove table #TableNumber*/
void RemoveTable(PDining_Room dining){		
	PTable temp;
	int TableNumber;			// table number to remove.
	float sum = 0, price;		// sum - total price to be paid by the table, temporary price.

	system("cls");
	printf("\nWhich table number to check out? ");
	_flushall();
	scanf("%d", &TableNumber);
	if (Check_TableNum(TableNumber))		// check if TableNumber is valid.
		return;
	TableNumber--;		// tables stored 0-14
	if (!dining->Tables[TableNumber])		// if table is empty (no portions ordered).
		printf("\n\nTable #%d is empty!", TableNumber + 1);
	else{
		printf("\n\nBill - Table #%d\n", TableNumber + 1);
		while (dining->Tables[TableNumber]){		// while portion exist.
			temp = dining->Tables[TableNumber];
			if (temp->HappyFlag == 1)			// if happy moment was activated on the portion.
				temp->price *= 0.75;            // 25% discount on the specific portion.
			price = temp->price;
			price *= temp->quantity;		// number of portions.
			printf("\n%s\t%dx%.2f$", temp->name, temp->quantity, temp->price);
			if (!temp->glutenFlag)
				printf(" (gluten-free)");
			if (temp->HappyFlag == 1)
				printf("\n\t\t(-25%% Happy Moment applied!)");
			sum += price;					// add up to total sum.
			dining->Tables[TableNumber] = dining->Tables[TableNumber]->next;
			/*free the portion's name and node*/
			free(temp->name);
			free(temp);
		}
		printf("\n\nTotal Price to pay: %.2f$\nDoes not include service!", sum);
		dining->Tables[TableNumber] = NULL;		//initialize the checked out table.
		dining->occupied--;						// decrease number of occupied tables.
	}
	getch();
}

/* get table number to transfer, find new empty table number.*/
void ChangeTable(PDining_Room dining){		
	int TableNumberS, TableNumberD = TABLES_NUM - 1;		// table number to transfer, highest table number.
	PTable temp;
	system("cls");
	printf("\nWhich table to tranfer? ");
	_flushall();
	scanf("%d", &TableNumberS);
	if (Check_TableNum(TableNumberS))		// check if TableNumberS is valid.
		return;
	TableNumberS--;		// tables stored 0-14
	temp = dining->Tables[TableNumberD];
	while (TableNumberD >= 0){			// find empty table with highest index possible.
		if (TableNumberD < 0){		// if no empty tables found
			printf("\n\nThere are no other avaliable tables!");
			getch();
			return;
		}
		else if (!dining->Tables[TableNumberD] && (TableNumberS != TableNumberD)){		/* if new table empty and 
tables number are different.*/
			if (!dining->Tables[TableNumberS])		// if current table to transfer is empty.
				printf("\n\nThere are no ordered items to table %d.", TableNumberS + 1);
			else{
				dining->Tables[TableNumberD] = dining->Tables[TableNumberS];
				dining->Tables[TableNumberS] = NULL;
			}
			printf("\nTable #%d moved to Table #%d.", TableNumberS + 1, TableNumberD + 1);
			getch();
			return;
		}
		TableNumberD--;
		temp = dining->Tables[TableNumberD];
		}
	}

/*Get kitchen lists and tables data, apply 25% discount on the most ordered portion.*/
void HappyMoments(PDining_Room dining, PKitchen_List list, PKitchen_List glutenFree){		
	int max, max2, i, j;		// quantity of highest ordered portions, index.
	PTable temp;
	int *CountArr_gluten, *CountArr_NoGluten;		// count arrays for number of orders.
	system("cls");
	printf("\n=====Happy Moment!!!===\n");
	if (dining->occupied){				// if restaurant have tables in use.
		CountArr_gluten = (int*)calloc(list->types, sizeof(int));
		if (!CountArr_gluten)
			Epic_Fail(dining, list, glutenFree);
		CountArr_NoGluten = (int*)calloc(glutenFree->types, sizeof(int));
		if (!CountArr_NoGluten)
			Epic_Fail(dining, list, glutenFree);
		printf("\n25%% discount applied on:\n");
		max = Count_Portions(dining, list, 1, CountArr_gluten);				// get quantity of most ordered regular product.
		max2 = Count_Portions(dining, glutenFree, 0, CountArr_NoGluten);		// get quantity of most ordered gluten free product.
		if (max < max2)
			max = max2;									// max = most popular quantity. (for both lists).
/*Find most popular portions and turn on their happy moment flag*/
		Turn_HappyON(dining, list, CountArr_gluten, max, 1);		// turn on flag for highest ordered products.
		Turn_HappyON(dining, glutenFree, CountArr_NoGluten, max, 0);		// turn on flag for highest ordered gluten free products.
		free(CountArr_gluten);
		free(CountArr_NoGluten);
	}
	else
		printf("\n\nThere are no orders found in the restuarant!");
	getch();
}

/*Get tables data and a kitchen list, find most ordered product in the list. returns the quantity of orders.*/
int Count_Portions(PDining_Room dining, PKitchen_List list, int glutenFlag, int *countArr){	
	PTable temp;
	PKitchen tempList;
	int i, j = 0, max = 0;	// indexs, quantity of orders, counter for a product.
	tempList = list->head;
	/*For each product in the kitchen list, scan all tables and for each table scan for the specific product 
	and add the quantity of orders to the counter.*/
	while (tempList){
		countArr[j] = 0;
		for (i = 0; i < TABLES_NUM; i++){
			temp = dining->Tables[i];
			while (temp){
				if (!strcmp(temp->name, tempList->name) && (temp->glutenFlag == glutenFlag))
					countArr[j] += temp->quantity;
				temp = temp->next;
			}
		}
		temp = dining->Trash;
		while (temp){				// trash list
			if (!strcmp(temp->name, tempList->name) && (temp->glutenFlag == glutenFlag))
				countArr[j] += temp->quantity;
			temp = temp->next;
		}
		if (max < countArr[j])
			max = countArr[j];
		tempList = tempList->next;
		j++;
	}
	return max;
}
/*Turn on happy moment flag for highest ordered products*/
void Turn_HappyON(PDining_Room dining, PKitchen_List list, int* CountArr, int max, int glutenFlag){		
	int i, j, printFlag = 0;
	PTable temp;
	PKitchen tempList = list->head;

	for (i = 0; i < list->types; i++){		// from 0 to size of arr
		if (max == CountArr[i]){			// if is the highest ordered product.
			for (j = 0; j < TABLES_NUM; j++){
				temp = dining->Tables[j];
				while (temp){		// scan for matched product, turn flag on
					if (!strcmp(tempList->name, temp->name) && (temp->glutenFlag == glutenFlag)){
						temp->HappyFlag = 1;
						printFlag = 1;
					}
					temp = temp->next;
				}
			}
		}
		if (printFlag){
			printf("%s", tempList->name);
			if (!glutenFlag)
				printf(" (gluten free)");
			printf(".\n");
			printFlag = 0;
		}
		tempList = tempList->next;
	}
}

/*Get tables data, cancel/remove an portion from a table.*/
int RemoveItem(PDining_Room dining){		
	int TableNumber, quantity, glutenFlag;		// table number, quantity to remove, indicate whether gluten free or not.
	char ProductName[MAX];						// temp product name.
	PTable temp, trash, temp2;
	int flag = 0;	// flag to indicate whether the table is empty after portion removal.
	char ch;
	system("cls");
	printf("\n===Portion canceling menu===\n");
	printf("\nEnter the table number: ");
	_flushall();
	scanf("%d", &TableNumber);
	if (Check_TableNum(TableNumber))	// check if TableNumber is valid
		return 0;
	TableNumber--;		// tables stored 0-14
	printf("\nEnter product to remove: ");
	_flushall();
	scanf("%s", ProductName);
	printf("\nWas %s a gluten-free product? (y- for yes): ", ProductName);
	_flushall();
	scanf("%c", &ch);
	if (ch == 'y' || ch == 'Y')
		glutenFlag = 0;
	else
		glutenFlag = 1;
	printf("\nHow many portions of %s to remove? ", ProductName);
	_flushall();
	scanf("%d", &quantity);
	if (Check_Sign(quantity, "Quantity"))
		return 0;
	temp = dining->Tables[TableNumber];
	while (temp){				// search for ProductName in table #TableNumber.
		if (!strcmp(ProductName, temp->name) && (temp->glutenFlag == glutenFlag)){		// if name and gluten flag matches
			if (quantity > temp->quantity){		// if quantity to remove exceeded the quantity of portions in the table.
				printf("\n\nThere are no %d of %s in Table #%d!", quantity, ProductName, TableNumber + 1);
				getch();
				return;
			}
			else{
				trash = (PTable)malloc(sizeof(Table));		//node for trash portion 
				if (!trash)
					return 1;
				trash->name = (char*)malloc((strlen(ProductName) + 1)*sizeof(char));
				if (!trash->name){
					free(trash);
					return 1;
				}
				strcpy(trash->name, ProductName);
				trash->glutenFlag = glutenFlag;
				trash->quantity = quantity;		// quantity removed
				trash->next = NULL;
				if (!dining->Trash){	// if trash list empty
					trash->prev = NULL;
					dining->Trash = trash;
				}
				else{						// if trash list not empty
					temp2 = dining->Trash;
					while (temp2->next)			// find where to add
						temp2 = temp2->next;
					temp2->next = trash;
					trash->prev = temp2;
				}
				
				temp->quantity -= quantity;		// subtract the quantity to cancel.
				if (!temp->quantity){		// if quantity = 0, remove the node.
					free(temp->name);
					if (temp->prev == NULL && temp->next == NULL)
						flag = 1;			// if was a single node, turn on a flag.
					/*link nodes*/
					if (temp->prev)
						temp->prev->next = temp->next;
					if (temp->next)
						temp->next->prev = temp->prev;
					free(temp);
					if (flag == 1){		// if portion removed was a single node, initialize table #TableNumber.
						dining->Tables[TableNumber] = NULL;
						dining->occupied--;
					}
				}
				printf("\n%dx%s removed from table #%d!", quantity, ProductName, TableNumber + 1);
				getch();
				return;
			}
		}
		else temp = temp->next;
	}
	if (!temp){			// if product not found
		printf("\n\nCannot find %s!", ProductName);
		getch();
		return;
	}
}

/* Get kitchen lists and tables data, remove a portion in a specific table.*/
void preOrderItem(PKitchen_List list, PKitchen_List glutenFree, PDining_Room dining){	
	char ch, opt, ProductName[MAX];		// user's key, temp product name.
	int glutenFlag, Errflag = 0, TableNumber;	// flags, table number.

	system("cls");
	printf("\n===Portion ordering menu===\n");
	printf("\nEnter the table number: ");
	_flushall();
	scanf("%d", &TableNumber);		// get table number (positive number < TABLE_NUM)
	if (Check_TableNum(TableNumber))
		return 0;
	TableNumber--;		// tables stored 0-14
	do{
		printf("\nMenu:\n");
		Print_Menu(list);
		printf("\nGluten free menu:\n");
		Print_Menu(glutenFree);
		printf("\nWhat product to order? ");
		_flushall();
		scanf("%s", ProductName);		// get a product's name
		printf("\nOrder gluten-free %s? (y for yes): ", ProductName);	// switch between gluten free and regular menu.
		_flushall();
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y'){
			glutenFlag = 0;
			Errflag = OrderItem(TableNumber, ProductName, glutenFlag, glutenFree, dining);
		}
		else{
			glutenFlag = 1;
			Errflag = OrderItem(TableNumber, ProductName, glutenFlag, list, dining);
		}
		if (Errflag == 1){	// if memory allocation failed
			Epic_Fail(dining, list, glutenFree);
			Error_Msg("Error allocating memory!");
		}
		printf("\n\nOrder another portion for table %d? (y - for yes): ", TableNumber + 1);
		_flushall();
		scanf("%c", &opt);
	} while (opt == 'y' || opt == 'Y');
	return 0;
}

/*Get table data, kitchen list, make an order for the table number.*/
int OrderItem(int TableNumber, char* ProductName, int glutenFlag, PKitchen_List list, PDining_Room dining){
	PKitchen temp = list->head;
	PTable Order, temp2;	// product to order, temp.
	int i, quantity;		// index, quantity to order.
	printf("\nHow many portions of %s? ", ProductName);
	_flushall();
	scanf("%d", &quantity);
	if (Check_Sign(quantity, "Quantity"))
		return 0;
	while (temp){
		if (!strcmp(temp->name, ProductName)){	// if found product to order
			if (quantity > temp->quantity){		// if not enough supplies in the kitchen.
				printf("\n\nNot enough %s in the kitchen!", ProductName);
				return 0;
			}
			temp2 = dining->Tables[TableNumber];
			while (temp2){
				if (!strcmp(temp2->name, ProductName) && (temp2->glutenFlag == glutenFlag)) {		// if Product Name already exists in the table.
					temp2->quantity += quantity;
					printf("\nQuantity of %s updated!", ProductName);
					return 0;
				}
				temp2 = temp2->next;
			}
			Order = (PTable)malloc(sizeof(Table));
			if (!Order)
				return 1;	// if memory allocattion failed.
			Order->name = (char*)malloc((strlen(ProductName) + 1)*sizeof(char));
			if (!Order->name){   // if memory allocattion failed.
				free(Order);
				return 1;
			}	
// copy Product data:
			strcpy(Order->name, ProductName);		
			Order->price = temp->price;
			Order->quantity = quantity;
			Order->glutenFlag = glutenFlag;
			temp->quantity -= quantity;
			Order->next = NULL;
			Order->HappyFlag = 0;
			if (dining->Tables[TableNumber] == NULL){		// if list empty
				dining->Tables[TableNumber] = Order;
				dining->Tables[TableNumber]->prev = NULL;
				dining->occupied++;
			}
			else{
				temp2 = dining->Tables[TableNumber];
				while (temp2->next)
					temp2 = temp2->next;	// get to the end of list.
				temp2->next = Order;
				Order->prev = temp2;
			}
			printf("\nOrdered %dx %s!", quantity, ProductName);
			return 0;
		}
		else
			temp = temp->next;		// keep searching for product name if exist.
	}
	if (!temp){			// if product not found
		printf("\n\n%s does not exist!", ProductName);
		return 0;
	}
}
/* decide which list to update, get quantity and name of product to add.*/
void preAddItems(PKitchen_List list, PKitchen_List glutenfree){		
	char ch, opt;
	system("cls");
	printf("\n===Kitchen Supplies adder===\n");
	do{
		printf("\nIs the product gluten-free? (y - for yes): ");
		_flushall();
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y')
			AddItems(glutenfree);		// add to gluten free list
		else
			AddItems(list);				// add to regualr list
		printf("\n\nAdd more supplies? (y - for yes): ");
		_flushall();
		scanf("%c", &opt);
	} while (opt == 'y' || opt == 'Y');
}

/*Add product quantity to a list*/
void AddItems(PKitchen_List list){	
	char prod[MAX];		// temp product name
	int quantity;
	PKitchen temp = list->head;
	printf("\nEnter product's name and quantity to add: ");
	_flushall();
	scanf("%s%d", prod, &quantity);
	if (Check_Sign(quantity, "Quantity"))		// check if quantity is valid
		return;

	while (temp){				// while list exist
		if (!strcmp(temp->name, prod)){	// if found product
			temp->quantity += quantity;		// update quantity
			printf("\nAdded %d of %s. %d are now in stock.",quantity, temp->name, temp->quantity);
			return;
		}
		temp = temp->next;
	}
	if (!temp)										// if product not found
		printf("\n\nCannot find %s!", prod);
}

/*get kitchen lists, choose list to update*/
int preCreateProduct(PKitchen_List list, PKitchen_List glutenFree){
	int ErrFlag = 0;
	char ch, opt;
	system("cls");
	printf("\n===Kitchen Supplies creator===\n");
	do{
		printf("\nIs the product gluten-free? (y - for yes): ");
		_flushall();
		scanf("%c", &ch);
		if (ch == 'y' || ch == 'Y')
			ErrFlag = CreateProduct(glutenFree);
		else
			ErrFlag = CreateProduct(list);
		if (ErrFlag)		// if memory allocation failed.
			return 1;
		else {
			printf("\nRegular list: ");
			Print_List(list);
			printf("\ngluten-free list: ");
			Print_List(glutenFree);
			printf("\n\nEnter another product? (y - for yes): ");
			_flushall();
			scanf("%c", &opt);
		}
		}while (opt == 'y' || opt == 'Y');
		return 0;
}

/* create a node in list with product data*/
int CreateProduct(PKitchen_List list){		
	PKitchen temp, prev;			// temporary
	PKitchen prod;
	char ProductName[MAX];			// temp product name
	float price;					// product's price
	int quantity, Errflag = 0;		// product's quantity, memory allocation flag.

	printf("\nEnter product's name, price and quantity: ");
	_flushall();
	scanf("%s%f%d", ProductName, &price, &quantity);	// get product data
	if (quantity < 0){
		printf("\n\nQuantity cannot be negative!");
		getch();
		return 0;
	}
	if (price <= 0){										// check if price if valid
		printf("\n\nInvalid price!");
		getch();
		return 0;
	}
	if (Search_Kitchen(list, ProductName))				// search if product already exist in the list.
		return 0;
	
	prod = (PKitchen)malloc(sizeof(Kitchen));			// allocate memory for a node
	if (!prod)
		return 1;
	prod->name = (char*)malloc((strlen(ProductName) + 1)*sizeof(char));	// allocate memory for a product's name
	if (!prod->name){
		free(prod);
		return 1;
	}
	strcpy(prod->name, ProductName);			// copy product's name
	prod->price = price;						// copy product's price
	prod->quantity = quantity;					// copy product's quantity
	list->types++;
	if (!list->head){		// if list empty
		prod->next = NULL;
		list->head = prod;
	}
	else{
		if (strcmp(prod->name, list->head->name) < 0) {  // add product to the head of list
			prod->next = list->head;
			list->head = prod;
		}
		else{
			temp = list->head->next;
			prev = list->head;
			if (temp)			//if list exist
				while (strcmp(prod->name, temp->name) > 0){		//find the location to insert prod.
					temp = temp->next;
					prev = prev->next;
					if (!temp)
						break;
				}
			prod->next = temp;		// link new product to the list.
			prev->next = prod;
		}
	}
	return 0;
}

/*Secondary functions*/

	void Free_Tables(PDining_Room dining) {		// remove all tables, free the allocated memory.
	int i;
	PTable temp;
	for (i = 0; i < TABLES_NUM; i++){
		while (dining->Tables[i]){
			temp = dining->Tables[i];
			dining->Tables[i] = dining->Tables[i]->next;
			free(temp->name);
			free(temp);
		}
		dining->Tables[i] = NULL;
	}
	while (dining->Trash){
		temp = dining->Trash;
		dining->Trash = dining->Trash->next;
		free(temp->name);
		free(temp);
	}
	dining->Trash = NULL;
}

int Search_Kitchen(PKitchen_List list, char* str){		// search for a product in a list. return 1 if found.
	PKitchen temp = list->head;
	while (temp){
		if (!strcmp(str, temp->name)){		// if productname found
			printf("\n\n%s already exists!", str);
			getch();
			return 1;
		}
		temp = temp->next;
	}
	return 0;
}

void initialize_Kitchen(PKitchen_List list){		// initialize a kitchen list.
	list->head = NULL;
	list->types = 0;
}

void Free_Kitchen(PKitchen_List list){		// delete a kitchen list. free the allocated memory.
	PKitchen temp;
	while (list->head){
		temp = list->head;
		list->head = list->head->next;
		free(temp->name);
		free(temp);
	}
	initialize_Kitchen(list);
}

void Print_Menu(PKitchen_List list){		// print a list as a menu .
	PKitchen temp = list->head;
	while (temp){
		printf("%s\t%.2f$\n", temp->name, temp->price);
		temp = temp->next;
	}
}

void Print_List(PKitchen_List list){		// print list as a linked list.
	PKitchen temp = list->head;
	while (temp){
		printf("%dx%s-->", temp->quantity, temp->name);
		temp = temp->next;
	}
}

void Epic_Fail(PDining_Room dining, PKitchen_List list, PKitchen_List glutenFree){	//free all allocated memory and exit(1).
	int i;
	Free_Kitchen(list);
	Free_Kitchen(glutenFree);
	Free_Tables(dining);
	Error_Msg("Error allocating memory!");
}

void initializeTables(PDining_Room dining){			// initialize tables in the dining room.
	int i;
	for (i = 0; i < TABLES_NUM; i++)
		dining->Tables[i] = NULL;
	dining->occupied = 0;
	dining->Trash = NULL;		// initialize trash list.
}

void Error_Msg(char *str){		// print error message and exit(1).
	printf("\n%s", str);
	getch();
	exit(1);
}

int Check_Sign(int num, char *str){		// get a number, description. if number is negative return 1.
	if (num <= 0){
		printf("\n\nInvalid %s!", str);
		getch();
		return 1;
	}
	else return 0;
}

int Check_TableNum(int num){			// check if table number is valid, returns 1 if NOT.
	if (num < 1 || num > TABLES_NUM){
		printf("\n\nTable #%d does not exist!", num);
		getch();
		return 1;
	}
	else return 0;
}

/*Get kitchen lists and tables data, print tables orders and the kitchen lists.*/
void Print_Status(PDining_Room dining, PKitchen_List list, PKitchen_List glutenFree){
	PKitchen temp;
	PTable temp2;
	int i;
	system("cls");
	printf("\n===Restaurant status===\n");
	temp = list->head;
	if (temp){
		printf("\nRegular products:");
		Print_List(list); 
	}
	printf("\n");
	temp = glutenFree->head;
	if (temp){
		printf("\nGluten free products:");
		Print_List(glutenFree);
	}
	printf("\n");
	for (i = 0; i < TABLES_NUM; i++){
		temp2 = dining->Tables[i];
		if (temp2){
			printf("\nTable #%d\n", i + 1);
			while (temp2){
				printf("%dx %s ", temp2->quantity, temp2->name);
				if (!temp2->glutenFlag)
					printf("(gluten free) ");
				if (temp2->HappyFlag)
					printf("~Happy Moment applied~");
				printf("\n");
				temp2 = temp2->next;
			}
		}
	}
	printf("\n");
	temp2 = dining->Trash;
	if (temp2)
		printf("Cancelled orders:\n");
	while (temp2){
		printf("%dx%s, ", temp2->quantity, temp2->name);
		temp2 = temp2->next;
	}
	getch();
}