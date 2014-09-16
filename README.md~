#Welcome to UHDT Software
####[Useful Coding Standard](http://users.ece.cmu.edu/~eno/coding/CCodingStandard.html)

##Important things to do
#####Use Header File Guards: 
All .h files must have this! This is needed to prevent redefinition of header files which will cause problems. Naming for header file guards is the same as macros.

Example: in flight_controller.h <br>

    #ifndef FLIGHT_CONTROLLER_H 
    #define FLIGHT_CONTROLLER_H
    **body of header file**
    #endif

#####Separate your files:
All functions should be declared in a .h file with an accompanying .c file for implementations.

#####Initialize your variables:
Try to initialize your variables to something (i.e. set pointers to NULL, integers to 0, etc.)

#####Limit Function Size:
Try to limit functions to less than a page of code.

#####Reduce Magic Numbers:
Try not to have magic numbers in code, use macros instead. This helps to increase readability.

Example:

Don't do this

    int i = 0;
    for (i = 0; i < 7; i++) 
    {
        do some stuff
    }

Do this

    int i = 0;
    for (i = 0; i < DAYS_OF_WEEK; i++) 
    {
        do some stuff
    }


##Coding Style
###Naming Conventions
#####Naming Files:
All lowercase, separated by underscores.

Example: 

    flight_controller.c

#####Naming Variables:
All lowercase, separated by underscores. For globals, put a "g_ " before the name.

Example:
    
    uint8_t num_of_secs = 0;
    float g_max_time = 0.0f;

#####Naming Functions
All lowercase, separated by underscores. First part of name should contain abbreviated file name.

Example: in pwm.c

    void pwm_init_timers();

#####Naming Macros:
All capital, separated by underscores.

Example:

    #define THIS_IS_A_MACRO -1


#####Naming Structs:
Every new word starts with a capital letter, no underscores.

Example: 

    typedef struct ExpensiveCar 
    {
        int car_body_type;
        int tires;
    } ExpensiveCar;

    ExpensiveCar tesla;

#####Naming Enums:
Enum names as same as struct. Every new word begins with a capital letter. Enum types are like macros, all uppercase and separated with underscores.

Example:

    enum FruitType
    {
        ORANGE,
        WATERMELON,
        GREEN_APPLE
    };

###Formatting

#####Brace Policy:
Braces should always be on a new line. 

Example:

    if (condition1)
    {
    }
    else if (condition2) 
    {
    }
    else
    {
    }

    while (condition) 
    {
    }

#####Spacing:
Tabs should be equal to 4 spaces. 

#####Switch/Case Formatting:
See example below.

Example:

    switch (...)
    {
        case 1:
            ...
        break;

        case 2:
            ...
        break;

        default:
    };
