/* ==========================================================
 * Green Valley Supply Company
 * Inventory Transaction Program
 * CCR-007 (Revised)
 *
 * Loads the current inventory from a text file if one exists
 * (otherwise prompts the employee for a starting value),
 * processes one customer purchase with a confirmation step,
 * displays a transaction summary, and saves the resulting
 * inventory back to the file so it persists between runs.
 * ========================================================== */

#include <stdio.h>

#define INVENTORY_FILE "inventory.txt"
#define NO_SAVED_INVENTORY (-1)
#define MAX_INVENTORY 1000

/* Global variable: the current inventory must remain available
   throughout the entire program, per the customer's request. */
int currentInventory = 0;

/* Function prototypes */
int loadInventory(void);
void saveInventory(int inventoryValue);
int getValidInventory(void);
int getValidPurchase(int available);
int askConfirmSale(int quantityPurchased, int available);
int askRestock(void);
int readWholeNumber(const char *prompt);
void clearInputBuffer(void);
void displayReport(int before, int sold, int remaining);

int main(void) {
  /* Local to this transaction only, as requested */
  int loadedInventory;
  int inventoryBefore;
  int quantityPurchased;
  int quantitySold;
  int inventoryRemaining;
  int saleConfirmed;

  /* Try to load a previously saved inventory; fall back to
     manual entry if no valid saved value is available */
  loadedInventory = loadInventory();

  if (loadedInventory == NO_SAVED_INVENTORY) {
    printf(
        "No saved inventory found. Please enter the starting inventory.\n\n");
    currentInventory = getValidInventory();
  } else {
    currentInventory = loadedInventory;
    printf("Loaded saved inventory: %d unit(s).\n\n", currentInventory);
  }

  inventoryBefore = currentInventory;

  /* If there is nothing in stock, offer the employee a chance
     to restock before continuing (a purchase of 0 is not
     allowed, so there is no valid sale to process otherwise) */
  if (currentInventory == 0) {
    printf("Inventory is empty.\n");

    if (!askRestock()) {
      printf("No restock performed. Exiting program.\n");
      saveInventory(currentInventory);
      return 0;
    }

    currentInventory = getValidInventory();
    printf("Inventory updated to %d unit(s).\n\n", currentInventory);
    inventoryBefore = currentInventory;
  }

  /* Get the quantity purchased for this transaction (local) */
  quantityPurchased = getValidPurchase(currentInventory);

  /* Let the employee cancel before anything is updated */
  saleConfirmed = askConfirmSale(quantityPurchased, currentInventory);

  if (!saleConfirmed) {
    printf("\nSale canceled. Inventory unchanged.\n");
    saveInventory(currentInventory);
    return 0;
  }

  /* Purchases greater than inventory were already rejected,
     so inventory can never go negative here */
  quantitySold = quantityPurchased;
  currentInventory = currentInventory - quantitySold;
  inventoryRemaining = currentInventory;

  displayReport(inventoryBefore, quantitySold, inventoryRemaining);

  /* Persist the resulting inventory for the next execution */
  saveInventory(currentInventory);

  return 0;
}

/* Attempts to load a saved inventory value from the text file.
   Returns the value if it is present and valid (0 to MAX_INVENTORY),
   or NO_SAVED_INVENTORY if the file is missing, empty, or corrupted. */
int loadInventory(void) {
  FILE *filePointer;
  int savedValue;
  int readResult;

  filePointer = fopen(INVENTORY_FILE, "r");

  if (filePointer == NULL) {
    return NO_SAVED_INVENTORY;
  }

  readResult = fscanf(filePointer, "%d", &savedValue);
  fclose(filePointer);

  if (readResult != 1 || savedValue < 0 || savedValue > MAX_INVENTORY) {
    return NO_SAVED_INVENTORY;
  }

  return savedValue;
}

/* Saves the current inventory value to the text file so it
   persists after the program terminates. */
void saveInventory(int inventoryValue) {
  FILE *filePointer;

  filePointer = fopen(INVENTORY_FILE, "w");

  if (filePointer == NULL) {
    printf("Warning: Unable to save inventory to file.\n");
    return;
  }

  fprintf(filePointer, "%d\n", inventoryValue);
  fclose(filePointer);
}

/* Prompts for and validates the starting inventory.
   Must be a whole number from 0 to 1000. Re-prompts on invalid input. */
int getValidInventory(void) {
  int value;
  int valid = 0;

  while (!valid) {
    value = readWholeNumber("Enter current inventory (0-1000): ");

    if (value < 0) {
      printf("Error: Inventory cannot be negative. Please try again.\n\n");
    } else if (value > 1000) {
      printf("Error: Inventory cannot exceed 1000. Please try again.\n\n");
    } else {
      valid = 1;
    }
  }

  return value;
}

/* Prompts for and validates the quantity purchased.
   Must be a whole number of at least 1, and cannot exceed the
   available inventory. Re-prompts on invalid input. */
int getValidPurchase(int available) {
  int value;
  int valid = 0;

  while (!valid) {
    value = readWholeNumber("Enter quantity purchased: ");

    if (value < 0) {
      printf("Error: Quantity purchased cannot be negative. Please try "
             "again.\n\n");
    } else if (value == 0) {
      printf("Error: A customer must purchase at least one product. Please try "
             "again.\n\n");
    } else if (value > available) {
      printf("Error: Cannot purchase more than the available inventory (%d in "
             "stock). Please try again.\n\n",
             available);
    } else {
      valid = 1;
    }
  }

  return value;
}

/* Asks the employee to confirm or cancel the sale before the
   inventory is updated. Uses a switch statement to evaluate the
   employee's Y/N response. Returns 1 to confirm, 0 to cancel. */
int askConfirmSale(int quantityPurchased, int available) {
  char choice;
  int confirmed = -1;

  printf("\nCustomer wants to purchase %d unit(s). Current inventory: %d.\n",
         quantityPurchased, available);

  while (confirmed == -1) {
    printf("Confirm this sale? (Y = confirm, N = cancel): ");
    scanf(" %c", &choice);
    clearInputBuffer();

    switch (choice) {
    case 'Y':
    case 'y':
      confirmed = 1;
      break;
    case 'N':
    case 'n':
      confirmed = 0;
      break;
    default:
      printf("Error: Please enter Y or N.\n\n");
      break;
    }
  }

  return confirmed;
}

/* Asks the employee whether they want to restock when inventory
   is empty. Uses a switch statement, same style as askConfirmSale.
   Returns 1 to restock, 0 to decline. */
int askRestock(void) {
  char choice;
  int wantsRestock = -1;

  while (wantsRestock == -1) {
    printf("Would you like to restock? (Y/N): ");
    scanf(" %c", &choice);
    clearInputBuffer();

    switch (choice) {
    case 'Y':
    case 'y':
      wantsRestock = 1;
      break;
    case 'N':
    case 'n':
      wantsRestock = 0;
      break;
    default:
      printf("Error: Please enter Y or N.\n\n");
      break;
    }
  }

  return wantsRestock;
}

/* Displays the given prompt, then reads one whole number from input.
   Rejects non-numeric input and decimal (float) input, re-prompting
   internally until a syntactically valid whole number is entered.
   Negative numbers are allowed through here so the calling function
   can give a specific "cannot be negative" message. */
int readWholeNumber(const char *prompt) {
  int value;
  int scanResult;
  char nextChar;
  int gotValidFormat = 0;

  while (!gotValidFormat) {
    printf("%s", prompt);
    scanResult = scanf("%d", &value);

    if (scanResult != 1) {
      printf("Error: Please enter numeric digits only.\n\n");
      clearInputBuffer();
      continue;
    }

    /* Look at what follows the number, skipping spaces/tabs */
    nextChar = getchar();
    while (nextChar == ' ' || nextChar == '\t') {
      nextChar = getchar();
    }

    if (nextChar == '.') {
      printf("Error: Whole numbers only, no decimals.\n\n");
      clearInputBuffer();
      continue;
    }

    if (nextChar != '\n' && nextChar != EOF) {
      printf("Error: Invalid characters in input.\n\n");
      clearInputBuffer();
      continue;
    }

    gotValidFormat = 1;
  }

  return value;
}

/* Flushes any leftover characters in the input buffer, up through
   the next newline, so bad input doesn't corrupt the next read. */
void clearInputBuffer(void) {
  char c;

  while ((c = getchar()) != '\n' && c != EOF) {
    /* discard */
  }
}

/* Displays the final transaction summary report */
void displayReport(int before, int sold, int remaining) {
  printf("\n----------------------------------------\n");
  printf("Green Valley Supply Company\n");
  printf("Inventory Transaction Summary\n");
  printf("----------------------------------------\n");
  printf("Inventory Before Sale : %d\n", before);
  printf("Products Sold : %d\n", sold);
  printf("Inventory Remaining : %d\n", remaining);
}
