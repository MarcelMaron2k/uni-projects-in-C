#ifndef _headers_H
#define _headers_H

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define NAME 50

typedef struct meal
{
	char* name;
	int amount;
	struct meal* next;
}meal;

typedef struct table
{
	int id;
	float bill;
	meal* head;

}table;

typedef struct menu
{
	char* ProductName;
	float Price;
	int Quantity;
	struct menu* next;
}menu;

// declarations on functions:
menu* CreateProducts(FILE*, FILE*); // creates the menu in the kitchen
void ErrorMsgExit(FILE*, char*); // send an error msg and exit the program
void ErrorMsg(FILE*, char*); // sends an error msg without exiting the program
void DeleteMenu(menu*); // free all allocation we did for the nodes and for the names of products
void AddItems(FILE*, menu*, char*, int); // adds an item to the menu
table* CreateTables(FILE*,FILE*,menu*, int); // create an array of tables, each table can have a linked list of ordered meals
void OrderItem(FILE*, table*, menu*, char*, int, int, int); // order an item from the menu and add it to the table list
meal* CreateOrder(FILE*, table*, table, menu*, int); //create a linked list for specified table and order
void RemoveItem(FILE*, menu*, char*, int, int, table*, int); // remove desired meal from the table order
int GetPrice(menu*, char*); // get the price of a product
void MenuDisplay(FILE*, menu*); // print the products and its quantities in the kitchen
void DeleteTable(meal*); // closes a table (delete all the linked list order)
int InMenu(menu*, char*); // search for a meal that already in menu
#endif