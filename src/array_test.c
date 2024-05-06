#include "../include/defs.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number_of_elements>\n", argv[0]);
        return 1;
    }
    
    int n = atoi(argv[1]);

    Registo myRegistos[n];

    Registo newRegisto;
    printf("1\n");
    for (int i = 0; i < n; i++) {
        newRegisto.id = i + 1;
        snprintf(newRegisto.args, sizeof(newRegisto.args), "argument%d", i + 1);
        newRegisto.duration = (i + 1) * 5.5; // Just an example, you can assign any value
        printf("%d %s %Lf\n", newRegisto.id, newRegisto.args, newRegisto.duration);
        addElement(&myRegistos, n, newRegisto); // Assigning directly to array element
    }

    printf("%d %s %Lf\n", myRegistos[0].id, myRegistos[0].args, myRegistos[0].duration);
    removeElement(myRegistos, n, &myRegistos[3]);

    printf("2\n");
    if (!isArrEmpty(myRegistos, n)) {
        printf("3\n");
        printf("Executing:\n");
        for (int i = 0; i < n; i++) {
            if (myRegistos[i].id != -1) { // Check if slot is not empty
                printf("%d %s\n", myRegistos[i].id, myRegistos[i].args);
            } 
        }
    } else {
        printf("Waiting:\n");
    }

    

    printf("4\n");

    return 0;
}
