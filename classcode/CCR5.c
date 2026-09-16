/*
 * Downtown Parking Services - Daily Parking Summary
 * CCR-005
 *
 * Employees enter fractional hours parked (down to quarter hours) for
 * each vehicle. Charges are $2.00 per hour, with no charge for stays
 * under a quarter hour, and a $50.00 cap per vehicle. Negative hours
 * and invalid/non-numeric input are rejected and re-prompted. Up to
 * MAX_VEHICLES may be processed per day, and a day with zero vehicles
 * reports $0.00 totals.
 */

#include <stdio.h>

int main(void) {
  const double RATE_PER_HOUR = 2.00;
  const double QUARTER_HOUR = 0.25;
  const double MAX_VEHICLE_CHARGE = 50.00;
  const int MAX_VEHICLES = 50;

  double hoursParked;
  double vehicleCharge;
  int vehicleCount = 0;
  double totalRevenue = 0.0;
  double averageFee;
  char moreVehicles;
  int scanResult;
  int ch;
  int endOfInput = 0;

  while (vehicleCount < MAX_VEHICLES) {
    /* --- Ask More vehicles? (Y/N), validated --- */
    while (1) {
      printf("More vehicles? (Y/N): ");
      scanResult = scanf(" %c", &moreVehicles);

      /* discard any leftover characters on the line (e.g. "Yes") */
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;

      if (scanResult != 1) {
        endOfInput = 1;
        break;
      }

      if (moreVehicles == 'Y' || moreVehicles == 'y' || moreVehicles == 'N' ||
          moreVehicles == 'n') {
        break;
      }

      printf("Please enter Y or N.\n");
    }

    if (endOfInput == 1) {
      break;
    }

    if (moreVehicles == 'N' || moreVehicles == 'n') {
      break;
    }

    /* --- Ask Hours Parked, validated --- */
    while (1) {
      printf("Vehicle %d Hours Parked: ", vehicleCount + 1);
      scanResult = scanf("%lf", &hoursParked);

      /* discard any leftover characters on the line (e.g. "5x") */
      while ((ch = getchar()) != '\n' && ch != EOF)
        ;

      if (scanResult != 1) {
        printf("Invalid input. Please enter a number.\n");
        continue;
      }

      if (hoursParked < 0) {
        printf("Hours parked cannot be negative. Please try again.\n");
        continue;
      }

      break;
    }

    vehicleCount++;

    if (hoursParked < QUARTER_HOUR) {
      vehicleCharge = 0.00;
    } else {
      vehicleCharge = hoursParked * RATE_PER_HOUR;

      if (vehicleCharge > MAX_VEHICLE_CHARGE) {
        vehicleCharge = MAX_VEHICLE_CHARGE;
      }
    }

    totalRevenue += vehicleCharge;

    printf("Vehicle %d Charge : $%.2f\n", vehicleCount, vehicleCharge);

    if (vehicleCount == MAX_VEHICLES) {
      printf("Maximum of %d vehicles reached for today.\n", MAX_VEHICLES);
      break;
    }
  }

  if (vehicleCount == 0) {
    averageFee = 0.00;
  } else {
    averageFee = totalRevenue / vehicleCount;
  }

  printf("----------------------------------------\n");
  printf("Downtown Parking Services\n");
  printf("Parking Summary\n");
  printf("----------------------------------------\n");
  printf("Vehicles Processed : %d\n", vehicleCount);
  printf("Total Revenue : $%.2f\n", totalRevenue);
  printf("Average Parking Fee: $%.2f\n", averageFee);

  return 0;
}
