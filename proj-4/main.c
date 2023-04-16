#include "headers.h"

int main()
{
	menu* menu_list = NULL;
	table* table_list = NULL;
	FILE* manot, * out, * Instructions;
	int tableSize = 0, input, stock ,table_number, amount, i;
	char name[NAME];

	// opening files and check if the action was successfull
	out = fopen("output.txt", "wt");
	if (!out)
		ErrorMsgExit(NULL, "Error in opening file!");

	manot = fopen("Manot.txt", "rt");
	if (!manot)
		ErrorMsgExit(out,"Error in opening Manot.txt file!");

	Instructions = fopen("Instructions.txt", "rt");
	if (!Instructions)
		ErrorMsgExit(out, "Error in opening Instructions.txt file!");

	fscanf(Instructions, "%d", &tableSize); // get the number of tables
	if (tableSize <= 0)
		ErrorMsgExit(out, "Table size can not be less than zero!!!");
	table_list = CreateTables(Instructions,out, menu_list, tableSize);
	fscanf(Instructions, "%d", &input); // get the action number
	if (input != 1)
	{
		free(table_list);
		ErrorMsgExit(out, "Must be the '1' first, in order to create the menu and the resturant!");
	}

	do
	{
		switch (input)
		{
			case 1:
				menu_list = CreateProducts(out, manot);
				break;

			case 2:
				fscanf(Instructions, "%s %d",name, &stock);
				AddItems(out, menu_list, name, stock);
				break;

			case 3:
				fscanf(Instructions, "%d %s %d",&table_number, name, &amount);
				OrderItem(out, table_list, menu_list, name, table_number, amount, tableSize);
				break;

			case 4:
				fscanf(Instructions, "%d %s %d", &table_number, name, &amount);
				RemoveItem(out, menu_list, name, table_number, amount, table_list, tableSize);
				break;

			case 5:
				MenuDisplay(out, menu_list);
				break;
		}
	} while (fscanf(Instructions, "%d", &input) == 1); // goes till the EOF

	for (i = 0; i < tableSize; i++) // free any open orders in the tables
	{
		if (table_list[i].head == NULL)
			continue;
		DeleteTable(table_list[i].head);
	}
	free(table_list);
	DeleteMenu(menu_list);
	fclose(manot);
	fclose(Instructions);
	fclose(out);
	return 0;
}