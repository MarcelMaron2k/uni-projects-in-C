#include "headers.h"

void AddItems(FILE* out, menu* node, char* name, int stock) {
	menu* head = node;
	if (stock <= 0)
	{
		ErrorMsg(out, "Quantity must be positive!\n");
		return;
	}
	while (head) {
		if (!strcmp(name, head->ProductName)) { // search for the desirable meal in menu
			head->Quantity += stock; // add the new amount to the meal that found
			fprintf(out, "%d %s were added to the kitchen!\n", stock, name);

			return;
		}

		head = head->next; // going for the next node in linked list menu
	}
	ErrorMsg(out, "Could not find specified meal!\n");
}

menu* CreateProducts(FILE* out, FILE* manot)
{
	menu* head = NULL;
	menu* node = NULL;
	char temp[NAME];
	float price;
	int stock , i = 0;
	while (fscanf(manot, "%s %d %f", temp, &stock, &price) == 3) // get values from file
	{
		if (price < 0)
		{
			ErrorMsg(out, "Price must be positive!\n");
			continue;
		}
		if (stock < 0)
		{
			ErrorMsg(out, "Quantity must be positive!\n");
			continue;
		}
		if (i == 1 && InMenu(head, temp)) // check for the second meal and on in menu
		{
			ErrorMsg(out, "Product already in kitchen!\n");
			continue;
		}
		node = (menu*)malloc(sizeof(menu)); // creating nodes for the list menu
		if (!node) // an error occured
		{
			DeleteMenu(head);
			ErrorMsgExit(out,"Error in allocation");
		}
		node->ProductName = (char*)malloc(strlen(temp) + 1); // allocating mem for name of product
		if (!(node->ProductName))
		{
			DeleteMenu(head);
			ErrorMsgExit(out,"Error in allocation");
		}
		strcpy(node->ProductName, temp);// copying the name into the node menu
		node->Quantity = stock;
		node->Price = price;
		node->next = head;
		head = node; // updating the head of the list
		i = 1;
	}
	fprintf(out, "The kitchen was created!\n");
	return head;
}

void ErrorMsgExit(FILE* out, char* str)
{
	if (out == NULL) // writing the error into the console
		printf("%s", str);
	else // writing the error into output file
		fprintf(out, "%s", str);
	exit(1);
}

void DeleteMenu(menu* head)
{
	menu* temp;
	while (head) // still have nodes in the list
	{
		temp = head;
		head = head->next; // updating the head
		temp->next = NULL; // isolating in purpose to free the node
		free(temp->ProductName);
		free(temp);
	}
	return;
}

table* CreateTables(FILE* Instructions, FILE* out, menu* head, int tableSize)
{	int i;
	table* resturant;
	resturant = (table*)malloc(sizeof(table) * tableSize); // allocating an array of tables
	if (!resturant)
	{
		free(resturant);
		DeleteMenu(head);
		ErrorMsgExit(out, "Error in allocation");
	}
		
	for (i = 0; i < tableSize; i++) // numbering the tables in the array and initializing thier bill and head pointer
	{
		resturant[i].id = i + 1;
		resturant[i].bill = 0;
		resturant[i].head = NULL;
	}
	return resturant;
}

void OrderItem(FILE* out, table* res, menu* menu_list, char* name, int num, int stock, int resturantSize)
{
	menu* menu_temp = menu_list;
	meal* meal_temp = res[num - 1].head;
	if (num > resturantSize) { // check the table number entered correctlly
		fprintf(out, "Entered table number (%d) exceeds number of tables (%d) in resturant!\n", num, resturantSize);
		return;
	}
	if (stock < 0)
	{
		ErrorMsg(out, "Stock must be positive number!\n");
		return;
	}
	while (menu_temp)
	{
		if (!strcmp(name, menu_temp->ProductName)) // check if the desired product is in the menu
			break;
		menu_temp = menu_temp->next; // search the product in the next node
	}
	if (!menu_temp || menu_temp->Quantity < stock) // check if the meal is in menu and the amout is avaliable in the menu
	{
		fprintf(out, "We dont have %s, sorry!\n", name);
		return;
	}
	menu_temp->Quantity -= stock; // updating the quantity in the menu

	while (meal_temp)
	{
		if (!strcmp(meal_temp->name, name))
		{
			res[num - 1].bill += menu_temp->Price * stock;
			meal_temp->amount += stock;
			fprintf(out, "%d %s were added to the table number %d\n", stock, name, num);
			return;
		}
		meal_temp = meal_temp->next;
	}
	res[num - 1].head = CreateOrder(out,res,res[num - 1], menu_temp, stock); // updating the head in 'order list' of the table
	res[num - 1].head->amount = stock;  // updating tables bill and stock of the order
	res[num - 1].bill += menu_temp->Price * stock;
	fprintf(out, "%d %s were added to the table number %d\n", stock, name, num);
}

meal* CreateOrder(FILE* out,table* table_list,table res, menu* menu, int stock)
{
	meal* food = (meal*)malloc(sizeof(meal)); // creating new node in order list
	if (!food)
	{
		free(table_list);
		DeleteMenu(menu);
		ErrorMsgExit(out, "Error in allocation");
	}	
	food->name = (char*)malloc(strlen(menu->ProductName) + 1); // allocating mem for the product name
	if (!food->name)
	{
		free(table_list);
		DeleteMenu(menu);
		ErrorMsgExit(out, "Error in allocation");
	}
	strcpy(food->name, menu->ProductName);
	food->amount = stock;
	food->next = res.head; // updating the head of the list
	res.head = food;
	return (res.head);
}

void RemoveItem(FILE* out, menu* menu,  char* ProductName, int TableNumber, int Quantity, table* table_list, int resturantSize) {
	meal* temp, *prevtemp;

	if (!table_list[TableNumber - 1].head) { // check the table has ordered before
		ErrorMsg(out, "The table does not have an order yet!\n");
		return;
	}
	if (!InMenu(menu, ProductName))
	{
		ErrorMsg(out, "Product is not in kitchen!\n");
		return;
	}
	if (Quantity < 0) { // check if the desired quantity to cancel is positive
		fprintf(out, "Entered Quantity (%d) must be greater than 0!\n", Quantity);
		return;
	}
	if (TableNumber >= resturantSize) { // check the table number entered correctlly
		fprintf(out, "Entered table number (%d) exceeds number of tables (%d) in resturant!\n", TableNumber, resturantSize);
		return;
	}

	prevtemp = table_list[TableNumber - 1].head;
	temp = prevtemp;

	while (temp) {
		if (!strcmp(ProductName, temp->name)) { // searching for the meal to cancel
			if (Quantity > temp->amount) { // check if ordered more meals than meals to cancel
				ErrorMsg(out, "Entered Quantity must be less or equal to than ordered quantity!\n");
				return;
			}
			else if (Quantity == temp->amount) { // check if the quantity to cancel equal to the ordered number
				prevtemp->next = temp->next;
				free(temp->name);
				free(temp); // free the current node
				table_list[TableNumber - 1].bill -= GetPrice(menu, ProductName) * Quantity; // update the bill
				return;
			}
			table_list[TableNumber-1].bill -= GetPrice(menu, ProductName) * Quantity; // update the bill and quantity
			table_list[TableNumber - 1].head->amount -= Quantity;
			fprintf(out, "%d %s was returned to the kitchen from table number %d\n", Quantity, ProductName, TableNumber);
			return;
		}
		prevtemp = temp; // uptating the pointers
		temp = temp->next;
	}

	fprintf(out, "Did not find specified order!\n");
}

int GetPrice(menu* menu_list, char* name) {
	int tempPrice = 0;
	menu* temp = menu_list;
	while (menu_list) {

		if (!strcmp(menu_list->ProductName, name)) { // search for the desired meal
			tempPrice = menu_list->Price;
			break; 
		}
		menu_list = menu_list->next;
	}

	menu_list = temp;
	return tempPrice; // return desired meal price
}

void MenuDisplay(FILE* out, menu* menu_list)
{
	menu* temp = menu_list;
	fprintf(out, "Kitchen: ");
	while (temp)
	{
		fprintf(out, "%s %d ", temp->ProductName, temp->Quantity); // prints the products and thier quantity
		temp = temp->next; // next node in the list
	}
}

void DeleteTable(meal* head)
{
	meal* temp;
	while (head) // still have nodes in the list
	{
		temp = head;
		head = head->next; // updating the head
		temp->next = NULL; // isolating in purpose to free the node
		free(temp->name);
		free(temp);
	}
	return;
}

int InMenu(menu* node, char* ProductName)
{
	menu* temp = node;
	while (temp)
	{
		if (!strcmp(temp->ProductName, ProductName)) // check if the product is in the kitchen
			return 1;
		temp = temp->next;
	}
	return 0;
}

void ErrorMsg(FILE* out, char* str)
{
	if (out == NULL)
		printf("%s", str);
	else
		fprintf(out, "%s", str);
}