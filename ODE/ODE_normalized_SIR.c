// Normalized SIR model
// ODE_normalized_SIR.c
// The latest version 2025/1/2

#include<stdio.h>
#include<math.h>
#include<stdlib.h>

#define BETA (double)(0.125)
#define GAMMA (double)(0.05)
#define DT (double)(0.05)
#define STEP 10000

double density_susceptible(double dns_S, double dns_I, double beta) {
	return -1 * beta * dns_S * dns_I;
}

double density_infectious(double dns_S, double dns_I, double beta, double gamma) {
	return beta * dns_S * dns_I - gamma * dns_I;
}

void fourth_runge_kutta(double* dns_S, double* dns_I, double* dns_R, double dt) {
	double k1_dns_S, k2_dns_S, k3_dns_S, k4_dns_S;
	double k1_dns_I, k2_dns_I, k3_dns_I, k4_dns_I;

	k1_dns_S = density_susceptible(*dns_S, *dns_I, BETA);
	k1_dns_I = density_infectious(*dns_S, *dns_I, BETA, GAMMA);
	k2_dns_S = density_susceptible(*dns_S + 5e-1 * dt * k1_dns_S, *dns_I + 5e-1 * dt * k1_dns_I, BETA);
	k2_dns_I = density_infectious(*dns_S + 5e-1 * dt * k1_dns_S, *dns_I + 5e-1 * dt * k1_dns_I, BETA, GAMMA);
	k3_dns_S = density_susceptible(*dns_S + 5e-1 * dt * k2_dns_S, *dns_I + 5e-1 * dt * k2_dns_I, BETA);
	k3_dns_I = density_infectious(*dns_S + 5e-1 * dt * k2_dns_S, *dns_I + 5e-1 * dt * k2_dns_I, BETA, GAMMA);
	k4_dns_S = density_susceptible(*dns_S + dt * k3_dns_S, *dns_I + dt * k3_dns_I, BETA);
	k4_dns_I = density_infectious(*dns_S + dt * k3_dns_S, *dns_I + dt * k3_dns_I, BETA, GAMMA);

	*dns_S = *dns_S + dt * (k1_dns_S + 2e0 * k2_dns_S + 2e0 * k3_dns_S + k4_dns_S) / 6e0;
	*dns_I = *dns_I + dt * (k1_dns_I + 2e0 * k2_dns_I + 2e0 * k3_dns_I + k4_dns_I) / 6e0;
	*dns_R = 1e0 - *dns_S - *dns_I;
}

int main(void) {
	char filename[20];
	int i;
	double t, dns_S, dns_I, dns_R;
	FILE *fp;

	// Open a file
	fp = fopen("result_SIR_ODE.dat", "w");
        if (fp == NULL) {
               printf("fail!\n");
        } else {
               printf("success!\n");
        }

	// Initial condition
	dns_I = 2.5e-2;
	dns_S = 1e0 - dns_I;
	dns_R = 0e0;
	t = 0e0;

	printf("BETA=%f, GAMMA=%f\n", BETA, GAMMA);
	printf("%f %f %f %f \n", t, dns_S, dns_I, dns_R);
	fprintf(fp, "%f %f %f %f \n", t, dns_S, dns_I, dns_R);

	// Calculation
	for (i = 1; i <= STEP; i++){
		t = i * DT;
		fourth_runge_kutta(&dns_S, &dns_I, &dns_R, DT);
		printf("%f %f %f %f\n", t, dns_S, dns_I, dns_R);
		fprintf(fp,"%f %f %f %f\n", t, dns_S, dns_I, dns_R);
	}

	fclose(fp);

	return 0;
}
