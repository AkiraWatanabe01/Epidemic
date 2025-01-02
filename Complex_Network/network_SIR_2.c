// SIR model on complex network
// SIR_net.c
// The latest version 2025/1/2

#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

#define NUM_NODE 4000					// Number of nodes
#define NUM_EDGE (NUM_NODE * (NUM_NODE - 1) / 2) 	// Number of edges
#define BETA (double)(0.125)				// Transmissibility
#define GAMMA (double)(0.05)				// Recovery rate
#define STEP 500					// Time step

double density(char *node, int number_node, char state_node) {
	int i;
        int total;
	
        total = 0;
        for (i = 0; i < number_node; i++) {
		if (node[i] == state_node) {
			total++;
		}
        }

        return (double)(total) / number_node;
}

int find_index(int num_small, int num_large) {
	return (NUM_NODE - 1) * num_small + num_large - num_small * (num_small + 1) / 2;
}

//===============MAIN================
int main(void){
	int i, j;
        int time_step;					// STEP
        int ID_first_node, ID_second_node;		// ID of nodes (first, second)
        int ID_node_small, ID_node_large;		// ID of nodes (small, large)
	int index_position;				// index
	char node[NUM_NODE];				// node, S:2, I:3, R:4
	char edge[NUM_EDGE];				// edge between nodes, Connect:1, Disconnect:0
        double density_S, density_I, density_R;		// density of each site
	double num_zero_one;				// random number [0, 1]
	FILE *fp;

// =========== Random number seed ==============
//	srand(time(NULL));
	srand(1);

// =========== Initial condition ==============
	for (i = 0; i < NUM_NODE; i++) {
		node[i] = 2;
	}

	for (i = 0; i < 100; i++) {
		node[i] = 3;
	}

	for (i = 0; i < NUM_EDGE; i++) {
		edge[i] = 1;
	}

	density_S=density(node, NUM_NODE, 2);
	density_I=density(node, NUM_NODE, 3);
	density_R=density(node, NUM_NODE, 4);
        printf("%f %f %f\n", density_S, density_I, density_R);

// =========== Open a file ============
	fp = fopen("result_SIR_net.dat", "w");
        if (fp == NULL) {
               printf("fail!\n");
        } else {
               printf("success!\n");
        }

// =========== Calculation ============
        fprintf(fp, "%d %f %f %f\n", 0, density_S, density_I, density_R);

        for(time_step = 1; time_step <= STEP; time_step++){
		for(i = 0; i < NUM_NODE; i++){
			ID_first_node = rand() % NUM_NODE;

			switch (node[ID_first_node]) {
			case 2:
				while (1) {
					ID_second_node = rand() % NUM_NODE;
					if (ID_first_node == ID_second_node) {
						continue;
					}

					ID_node_large = ID_first_node;
					ID_node_small = ID_second_node;
					if (ID_first_node < ID_second_node) {
						ID_node_large = ID_second_node;
						ID_node_small = ID_first_node;
					}
					index_position = find_index(ID_node_small, ID_node_large);

					if (edge[index_position] == 1) {
						break;
					}
				}

				if (node[ID_second_node] == 3) {
					num_zero_one = (double)rand() / RAND_MAX;
					if (num_zero_one <= BETA) {
						node[ID_first_node] = 3;
					}
				}
				break;
			case 3:
				num_zero_one = (double)rand() / RAND_MAX;
				if (num_zero_one <= GAMMA) {
					node[ID_first_node] = 4;
				}
				break;
			default:
				break;
			}
		}

		density_S=density(node, NUM_NODE, 2);
		density_I=density(node, NUM_NODE, 3);
		density_R=density(node, NUM_NODE, 4);
		printf("STEP:%3d, density_S=%f, density_I=%f, density_R=%f\n", time_step, density_S, density_I, density_R);
		fprintf(fp,"%d %f %f %f\n", time_step, density_S, density_I, density_R);
        }
	fclose(fp);

	return 0;
}
