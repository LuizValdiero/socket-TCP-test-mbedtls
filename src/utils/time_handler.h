
#include <time.h>
#include <stdio.h>

time_t second_to_micro(time_t second) {
    return second * 1000000;
}
time_t micro_to_second(time_t micro) {
    return micro / 1000000;
}

void print_date_second(time_t second) {

    char* c_time_string = ctime(&second);

    if (c_time_string == NULL)
    {
        printf("Failure to convert the current time.\n");
        return;
    }
    printf("date: %s\n", c_time_string);
}