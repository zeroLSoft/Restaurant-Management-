#include "Header.h"

void main(){
	Kitchen_List glutenFree_Products;		// gluten free products list.
	Kitchen_List Products;					// regular products list.
	Dining_Room Dining;						// Tables info of the restaurant.
	char opt;								// get key from user.
	int FailFlag = 0;						// memory allocating error flag.

	/*Initialize lists and tables*/
	initialize_Kitchen(&Products);
	initialize_Kitchen(&glutenFree_Products);
	initializeTables(&Dining);
	do{
		system("cls");
		printf("\n\n===Restaurant manager menu===\n");
		printf("\n1. Create kitchen products.");
		printf("\n2. Add supplies to kitchen.");
		printf("\n3. Order to a table.");
		printf("\n4. Cancel a table's order.");
		printf("\n5. Happy moment.");
		printf("\n6. Transfer a table.");
		printf("\n7. Check out a table.");
		printf("\n8. Print Restaurant status. (Kitchen lists and tables)");
		printf("\n\nPlease choose an option 1-8. Press f to exit program: ");
		_flushall();
		scanf("%c", &opt);
		printf("\n");
		switch (opt){
		case '1':
			FailFlag = preCreateProduct(&Products, &glutenFree_Products);
			if (FailFlag == 1)
				Epic_Fail(&Dining, &Products, &glutenFree_Products);
			break;
		case '2':
			preAddItems(&Products, &glutenFree_Products);
			break;
		case '3':
			preOrderItem(&Products, &glutenFree_Products, &Dining);
			break;
		case '4':
			FailFlag = RemoveItem(&Dining);
			if (FailFlag == 1)
				Epic_Fail(&Dining, &Products, &glutenFree_Products);
			break;
		case '5':
			HappyMoments(&Dining, &Products, &glutenFree_Products);
			break;
		case '6':
			ChangeTable(&Dining);
			break;
		case '7':
			RemoveTable(&Dining);
			break;
		case '8':
			Print_Status(&Dining, &Products, &glutenFree_Products);
			break;
		default:
			if (opt != 'f' && opt != 'F'){
				system("cls");
				printf("\nBad input! Please try again..");
				getch();
			}
		}
	} while (opt != 'f' && opt != 'F');
	
		/*Free all allocated memory*/
		Free_Kitchen(&Products);
		Free_Kitchen(&glutenFree_Products);
		Free_Tables(&Dining);
		printf("\nFinish.");
		getch();
}