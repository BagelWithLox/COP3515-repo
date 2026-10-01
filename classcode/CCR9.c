/*
 * box_report.c
 * CCR-009: Atlantic Shipping & Logistics - Package Measurement Report
 *
 * Reads the length, width and height (in inches) of one shipping box,
 * checks them against the shipping limits, and prints the volume and
 * surface area. Calculations are not rounded; results are displayed
 * with 2 decimal places (more only when a small value would otherwise
 * show as 0.00).
 *
 * Future versions will add shipping cost calculations. Keep the
 * calculation functions free of input/output so they can be reused.
 */

#include <stdio.h>

/* Shipping limits in inches:  39' x 7'5" x 7'6" */
#define MAX_LENGTH 468.0 /* 39 ft * 12          */
#define MAX_WIDTH 89.0   /* 7 ft * 12 + 5       */
#define MAX_HEIGHT 90.0  /* 7 ft * 12 + 6       */

/* ---------- Reusable calculation functions (no input/output) ---------- */

double calculate_volume(double length, double width, double height) {
  return length * width * height;
}

double calculate_surface_area(double length, double width, double height) {
  return 2.0 * (length * width + length * height + width * height);
}

/* ---------- Input ---------- */

/* Reads one number into *value. Returns 1 if it is valid
   (greater than 0 and not more than max), 0 if invalid (after printing
   an error and discarding the line), or -1 if input has ended. */
int read_dimension(double *value, double max) {
  int status;
  int ch;

  status = scanf("%lf", value);

  if (status == 1 && *value > 0.0 && *value <= max) {
    return 1;
  }

  if (status == EOF) {
    printf("\nError: no input received.\n");
    return -1;
  }

  if (status != 1) {
    printf("Error: please enter a number.\n");
  } else if (*value <= 0.0) {
    printf("Error: dimension must be greater than zero.\n");
  } else {
    printf("Error: exceeds the shipping limit of %.2f inches.\n", max);
  }

  while ((ch = getchar()) != '\n' && ch != EOF) {
  }
  return 0;
}

/* Prompts until a valid value is entered.
   Returns 1 on success, 0 if input ended before a valid value. */
int prompt_dimension(const char *label, double max, double *value) {
  int result = 0;

  while (result == 0) {
    printf("Enter box %s in inches (max %.2f): ", label, max);
    result = read_dimension(value, max);
  }
  return result == 1;
}

/* ---------- Output ---------- */

/* Returns the number of decimal places to display: 2 normally, but
   more for small values (under 0.01) so they do not show as 0.00.
   Small values are shown with 2 significant digits. */
int display_decimals(double value) {
  int places = 2;
  double scale = 100.0; /* 10 to the power of places */

  if (value > 0.0 && value < 0.01) {
    while (places < 30 && value * scale < 9.999999) {
      places++;
      scale = scale * 10.0;
    }
  }
  return places;
}

/* Display only; the stored value is not rounded. */
void print_value(const char *label, double value, const char *unit) {
  printf("%s : %.*f %s\n", label, display_decimals(value), value, unit);
}

void print_report(double length, double width, double height, double volume,
                  double surface_area) {
  printf("\n----------------------------------------\n");
  printf("Atlantic Shipping & Logistics\n");
  printf("Package Measurement Report\n");
  printf("----------------------------------------\n");
  print_value("Length", length, "inches");
  print_value("Width", width, "inches");
  print_value("Height", height, "inches");
  printf("-------------------------------\n");
  print_value("Volume", volume, "cubic inches");
  print_value("Surface Area", surface_area, "square inches");
}

/* ---------- Main ---------- */

int main(void) {
  double length, width, height;
  double volume, surface_area;

  if (!prompt_dimension("length", MAX_LENGTH, &length) ||
      !prompt_dimension("width", MAX_WIDTH, &width) ||
      !prompt_dimension("height", MAX_HEIGHT, &height)) {
    return 1;
  }

  volume = calculate_volume(length, width, height);
  surface_area = calculate_surface_area(length, width, height);

  print_report(length, width, height, volume, surface_area);
  return 0;
}
