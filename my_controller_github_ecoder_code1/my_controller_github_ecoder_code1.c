#include <math.h>
#include <webots/robot.h>
#include <webots/motor.h>
#include <webots/distance_sensor.h>
#include <stdio.h>
#include <webots/camera.h>
#include <webots/led.h>
#include <string.h>

#define TIME_STEP 64
#define MAX_SPEED 6.28

#define WHEEL_RADIUS 0.02  // Wheel radius in meters
#define WHEEL_DISTANCE 0.055 // Distance between wheels in meters

#define INITIAL_X 0.227693  // Initial X position in meters
#define INITIAL_Y 0.804154 // Initial Y position in meters

#define X_MIN 18
#define X_MAX 20
#define Y_MIN -5
#define Y_MAX 2
double max_speed = 6.28;
#define THRESHOLD 950


// Define turn_speed and turn_duration (adjust values as needed)
#define turn_speed 3.0 
#define turn_duration 0.5 // in seconds
#define turn_speed 3.0
void move_robot_turn(WbDeviceTag left_motor, WbDeviceTag right_motor, double left_speed, double right_speed, double duration) {
    // Get the current simulation time
    double start_time = wb_robot_get_time();

    // Turn the robot for the specified duration
    while (wb_robot_get_time() - start_time < duration) {
        wb_motor_set_velocity(left_motor, left_speed);
        wb_motor_set_velocity(right_motor, right_speed);
        wb_robot_step(TIME_STEP); // Step the simulation
    }



    // Stop the motors after turning
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_velocity(right_motor, 0.0);
}

void move_robot(WbDeviceTag left_motor, WbDeviceTag right_motor, double left_speed, double right_speed) {
    wb_motor_set_velocity(left_motor, left_speed);
    wb_motor_set_velocity(right_motor, right_speed);
}
// Function to calculate the robot's position based on the encoders
// void get_robot_position(WbDeviceTag left_encoder, WbDeviceTag right_encoder, double *delta_x, double *delta_y) {
    // double left_distance = wb_position_sensor_get_value(left_encoder) * WHEEL_RADIUS;
    // double right_distance = wb_position_sensor_get_value(right_encoder) * WHEEL_RADIUS;
    // double distance = (left_distance + right_distance) / 2.0;
    // double theta = (right_distance - left_distance) / WHEEL_DISTANCE;

    // *delta_x = distance * cos(theta);
    // *delta_y = distance * sin(theta);
// }


//leftwall
void left_wall_follow(double max_speed, int front_wall, int left_wall, int left_corner, int left_motor, int right_motor, int left_speed, int right_speed){

   // Decision making
      if (front_wall) {
          printf("Turn right in place\n");
          left_speed = max_speed;
          right_speed = -max_speed;
      } else {
          if (left_wall) {
              printf("Drive forward\n");
              left_speed = max_speed;
              right_speed = max_speed;
          } else {
              printf("Turn right\n");
              left_speed = max_speed / 8;
              right_speed = max_speed;
          }
          if (left_corner) {
              printf("Came too close, drive right\n");
              left_speed = max_speed;
              right_speed = max_speed / 8;
          }
      }

      // Set motor velocities
      wb_motor_set_velocity(left_motor, left_speed);
      wb_motor_set_velocity(right_motor, right_speed);

}

// Function declarations for motor control (implement these separately)
// Function prototypes
void move_robot(WbDeviceTag left_motor, WbDeviceTag right_motor, double left_speed, double right_speed);
void move_robot_turn(WbDeviceTag left_motor, WbDeviceTag right_motor, double left_speed, double right_speed, double duration);

// ... other function prototypes ...

//void turn_180(WbDeviceTag left_motor, WbDeviceTag right_motor) {
  //  move_robot_turn(left_motor, right_motor, -turn_speed, turn_speed, turn_duration); 
    //move_robot_turn(left_motor, right_motor, -turn_speed, turn_speed, turn_duration);// Turn 180
    //move_robot(left_motor, right_motor, 3.0, 3.0); // Continue after turn
//}

void turn_180(WbDeviceTag left_motor, WbDeviceTag right_motor) {
    // Calculate the duration needed for a 180-degree turn
    double turn_duration_180 = (M_PI * WHEEL_DISTANCE) / (2.0 * turn_speed * WHEEL_RADIUS); // Assuming differential drive kinematics
   // move_robot(left_motor, right_motor, 3.0, 3.0);
    // Execute the turn for the calculated duration
    move_robot_turn(left_motor, right_motor, -turn_speed, turn_speed, turn_duration_180); 
}
void turn_right(WbDeviceTag left_motor, WbDeviceTag right_motor) {
    // Calculate the duration needed for a 90-degree turn
    double turn_duration_90 = (M_PI_2 * WHEEL_DISTANCE) / (2.0 * turn_speed * WHEEL_RADIUS); // Assuming differential drive kinematics
    //move_robot(left_motor, right_motor, 3.0, 3.0);
    // Execute the turn for the calculated duration
    move_robot_turn(left_motor, right_motor, turn_speed, -turn_speed, turn_duration_90);
}
void turn_left(WbDeviceTag left_motor, WbDeviceTag right_motor) {
    double turn_duration_90 = (M_PI_2 * WHEEL_DISTANCE) / (2.0 * turn_speed * WHEEL_RADIUS);
    move_robot_turn(left_motor, right_motor, -turn_speed, turn_speed, turn_duration_90);
   // move_robot_turn(left_motor, right_motor, -turn_speed, turn_speed, turn_duration); // Turn left
   // move_robot(left_motor, right_motor, 3.0, 3.0); // Continue after turn
}

void move_forward(WbDeviceTag left_motor, WbDeviceTag right_motor) {
    move_robot(left_motor, right_motor, 3.0, 3.0);
}

// Function to detect color from the camera
const char *detect_color(const unsigned char *image, int width, int height) {
    int red = 0, green = 0, blue = 0;

    // Average the RGB values from the camera image
    for (int i = 0; i < width * height * 4; i += 4) {
        red += image[i];
        green += image[i + 1];
        blue += image[i + 2];
    }

    int pixels = width * height;
    red /= pixels;
    green /= pixels;
    blue /= pixels;
    
    printf("Average red value: %d\n", red);
    printf("Average green value: %d\n", green);
    printf("Average blue value: %d\n", blue);

    // Identify color based on RGB thresholds
    if (blue > 200 && green < 100 && red < 100)
        return "RED";
    if (red < 200 && green > 200 && blue > 200)
        return "YELLOW";
    if (red > 200 && green < 100 && blue > 200)
        return "PINK";
    if (red < 100 && green < 100 && blue > 100)
        return "BROWN";
    if (red < 100 && green > 200 && blue < 100)
        return "GREEN";

    return "NONE";
}


//junction_directions_pink_to_red[] = {
  //  {{1, 0, 0}, "180"},  // Example: Turn left when left is open, front is blocked, right is blocked
    //{{0, 1, 1}, "right"},
   // {{0, 0, 1}, "left"},
  //  {{0, 0, 1}, "right"},
  //  {{0, 0, 1}, "right"},
  //  {{1, 0, 0}, "left"},
   // {{1, 0, 0}, "left"}    
    
     // Example: Turn right when left and front are blocked, right is open
    // ... Add more junction directions for the RED to GREEN sequence
//};
//typedef struct {
  //  int sensor_readings[3];
    //const char *direction;
//} JunctionData;

// Junction directions for PINK to RED sequence
//JunctionData junction_directions_pink_to_red[] = {
  //  {{1, 0, 0}, "180"},  // Eleft.front,right
  //  {{0, 1, 1}, "right"},
  //  {{1, 1, 0}, "left"}, // Example: Turn right when left and front are blocked, right is open
  //  {{0, 0, 1}, "left"},
  //  {{0, 0, 1}, "right"},
   // {{0, 0, 1}, "right"},
   // {{1, 0, 0}, "left"}
    
    // ... Add more junction directions 
//};

// Function to detect a junction 
int is_at_junction(int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    // Adjust these conditions based on your sensor thresholds and junction layout
    if (left_obstacle_value == 1 && front_obstacle_value == 0 && right_obstacle_value == 0) {
         return 1; // Turn left when left is open, front is blocked, right is blocked
         
     } else if (left_obstacle_value == 0 && front_obstacle_value == 1 && right_obstacle_value == 1) {
         return 2;  // Turn right when right is open, front is blocked, right is blocked
     } else if (left_obstacle_value == 0 && front_obstacle_value == 0 && right_obstacle_value == 1) {
         return 3; // Turn right when left and front are blocked, right is open
     } else if (left_obstacle_value == 1 && front_obstacle_value == 1 && right_obstacle_value == 0){
          return 4;//left and front are open
     }else if (left_obstacle_value == 1 && front_obstacle_value == 0 && right_obstacle_value == 1){
          return 5;//left and right are open
     }else if (left_obstacle_value == 0 && front_obstacle_value == 1 && right_obstacle_value == 1){
          return 6;//front and right are open
     }else if (left_obstacle_value == 1 && front_obstacle_value == 1 && right_obstacle_value == 1){
          return 7;//all are open
     }

    return 0; // Not at a junction
}


//void red_to_green(int left_obstacle_value,int front_obstacle_value,int right_obstacle_value){

  //store passed values of the junction along with a [(1,0,1),'left'],[(1,0,1),'left']...
  //match with first case ask to turn left

//}
//void yellow_to_red(int left_obstacle_value,int front_obstacle_value,int right_obstacle_value){
  

//}
//void pink_to_red(int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    // Current junction sensor readings
  //  int current_junction[] = {left_obstacle_value, front_obstacle_value, right_obstacle_value};

    // Iterate through junction directions
    //for (int i = 0; i < sizeof(junction_directions_pink_to_red) / sizeof(junction_directions_pink_to_red[0]); i++) {
        // Check if current sensor readings match a stored junction
      //  if (memcmp(current_junction, junction_directions_pink_to_red[i][0], sizeof(current_junction)) == 0) {
            // Execute the corresponding direction
        //    if (strcmp(junction_directions_pink_to_red[i][1], "180") == 0) {
          //      turn_180();
          //  } else if (strcmp(junction_directions_pink_to_red[i][1], "right") == 0) {
            //    turn_right();
           // } else if (strcmp(junction_directions_pink_to_red[i][1], "left") == 0) {
             //   turn_left();
            //}else if (strcmp(junction_directions_pink_to_red[i][1], "right") == 0) {
              //  turn_right();
            //} else if (strcmp(junction_directions_pink_to_red[i][1], "right") == 0) {
              // turn_right();
            //}else if (strcmp(junction_directions_pink_to_red[i][1], "left") == 0) {
             //   turn_left();
            //} else if (strcmp(junction_directions_pink_to_red[i][1], "left") == 0) {
              //  turn_left();
            //}
            // move_forward();
            // Junction handled, break the loop
            // return;
       // }
   // }

    // Handle cases where junction is not found in the predefined list (error handling)
    // ...
//}
//void pink_to_red(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
//void pink_to_red(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
  //  printf("I'm in pink to red function: %d %d %d\n", left_obstacle_value, front_obstacle_value, right_obstacle_value);
//
//    int current_junction[] = {left_obstacle_value, front_obstacle_value, right_obstacle_value};

 //   for (int i = 0; i < sizeof(junction_directions_pink_to_red) / sizeof(junction_directions_pink_to_red[0]); i++) {
        // Compare sensor readings with stored junction data
      //  if (memcmp(current_junction, junction_directions_pink_to_red[i].sensor_readings, sizeof(current_junction)) == 0) {
            
            // Execute the corresponding direction
    //        if (strcmp(junction_directions_pink_to_red[i].direction, "180") == 0) {
//                turn_180(left_motor, right_motor);
  //              printf("Turned 180 degrees\n");
         //   } else if (strcmp(junction_directions_pink_to_red[i].direction, "right") == 0) {
       //         turn_right(left_motor, right_motor);
     //            printf("Turned right\n");
           // } else if (strcmp(junction_directions_pink_to_red[i].direction, "left") == 0) {
              //  turn_left(left_motor, right_motor);
            //    printf("Turned left\n");
          //  }

        //    move_forward(left_motor, right_motor); // Move forward after turning (if needed)
      //      return; // Exit the loop after handling the junction
    //    }
  //  }
    //Handle the case if the junction not found in the array
     // ...
//}


//...................................



//........................................................................





void pink_to_red(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    static int turn_sequence_index = 0; // Static variable to track the turn sequence
    
   // static int turn_executed = 0; // Flag to indicate if a turn has been executed
    // Define the turn sequence
    const char *turn_sequence[] = {"180a", "righta", "left2a", "righta", "lefta", "left3a", "left3a"};
    int sequence_length = sizeof(turn_sequence) / sizeof(turn_sequence[0]);
  while (turn_sequence_index < sequence_length) {
   // Check if at a junction and a turn hasn't been executed yet
     if (is_at_junction(left_obstacle_value, front_obstacle_value, right_obstacle_value) != 0) {
        // Execute the current turn in the sequence
        if (strcmp(turn_sequence[turn_sequence_index], "180a") == 0) {
            turn_180(left_motor, right_motor);
            
            move_forward(left_motor, right_motor);
           
            wb_robot_step(5000);
            printf("Turned 180 degrees\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "righta") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4000);
            printf("Turned right\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "left2a") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(15500);
            printf("Turned left2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "lefta") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned left\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left3a") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(8000);
            printf("Turned left3\n");
        }


        turn_sequence_index++; // Move to the next turn in the sequence
       // turn_executed = 1; // Set the flag to indicate a turn has been executed
         
      }
    
    // Move forward after turning
    move_forward(left_motor, right_motor);
   } 
}

//..........................Green to red.................
void green_to_red(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    static int turn_sequence_index = 0; // Static variable to track the turn sequence
    
    //static int turn_executed = 0; // Flag to indicate if a turn has been executed
    // Define the turn sequence
    const char *turn_sequence[] = {"180", "right", "left", "right2", "right3", "left","right4"}; 
    int sequence_length = sizeof(turn_sequence) / sizeof(turn_sequence[0]);
  while (turn_sequence_index < sequence_length) {
   // Check if at a junction and a turn hasn't been executed yet
     if (is_at_junction(left_obstacle_value, front_obstacle_value, right_obstacle_value) != 0) {
        // Execute the current turn in the sequence
        if (strcmp(turn_sequence[turn_sequence_index], "180") == 0) {
            turn_180(left_motor, right_motor);
            
            move_forward(left_motor, right_motor);
           
            wb_robot_step(20500);
            printf("Turned 180 degrees\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "right") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(5000);
            printf("Turned right\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "right2") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(20000);
            printf("Turned right2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned left\n");
       
        }else if (strcmp(turn_sequence[turn_sequence_index], "right3") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(7000);
            printf("Turned right3\n");
         }else if (strcmp(turn_sequence[turn_sequence_index], "right4") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(1);
            printf("Turned right4\n");
         }
            turn_sequence_index++; // Move to the next turn in the sequence
       // turn_executed = 1; // Set the flag to indicate a turn has been executed
         
      }
    
    // Move forward after turning
    move_forward(left_motor, right_motor);
   } 
}

    // Move to the next turn in the sequence if a turn was executed
   // if (turn_executed) {
      ////  turn_sequence_index++;
       // turn_executed = 0; // Reset the flag for the next turn

        // Reset the sequence if it reaches the end
       // if (turn_sequence_index >= sizeof(turn_sequence) / sizeof(turn_sequence[0])) {
           // turn_sequence_index = 0;
      //  }
   // }
   // printf("turn_sequence_index: %d\n", turn_sequence_index);
//}

//..........................Yellow to red...............

void yellow_to_red(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    static int turn_sequence_index = 0; // Static variable to track the turn sequence
    
    //static int turn_executed = 0; // Flag to indicate if a turn has been executed
    // Define the turn sequence
    const char *turn_sequence[] = {"180", "left2", "right", "right", "left2", "right", "right","left","left","right2","right3","left", "left4", "right", "left", "left3", "left3"}; 

   // Check if at a junction and a turn hasn't been executed yet
    int sequence_length = sizeof(turn_sequence) / sizeof(turn_sequence[0]);
  while (turn_sequence_index < sequence_length) {
   // Check if at a junction and a turn hasn't been executed yet
     if (is_at_junction(left_obstacle_value, front_obstacle_value, right_obstacle_value) != 0) {
        // Execute the current turn in the sequence
        if (strcmp(turn_sequence[turn_sequence_index], "180") == 0) {
            turn_180(left_motor, right_motor);
            
            move_forward(left_motor, right_motor);
           
            wb_robot_step(8000);
            printf("Turned 180 degrees\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "right") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned right\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "left2") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(13000);
            printf("Turned left2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned left\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "right2") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(8000);
            printf("Turned right2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left3") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(8000);
            printf("Turned left3\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "right3") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(7000);
            printf("Turned right3\n");   
        }else if (strcmp(turn_sequence[turn_sequence_index], "left4") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(15500);
            printf("Turned left4\n");
        }
        turn_sequence_index++; // Move to the next turn in the sequence
       // turn_executed = 1; // Set the flag to indicate a turn has been executed
         
      }
    
    // Move forward after turning
    move_forward(left_motor, right_motor);
   } 
}

//...............Brown to red..........
void brown_to_red(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    static int turn_sequence_index = 0; // Static variable to track the turn sequence
    
    //static int turn_executed = 0; // Flag to indicate if a turn has been executed
    // Define the turn sequence
    const char *turn_sequence[] = {"180", "right", "left", "left3", "left3"}; 

   // Check if at a junction and a turn hasn't been executed yet
    int sequence_length = sizeof(turn_sequence) / sizeof(turn_sequence[0]);
  while (turn_sequence_index < sequence_length) {
   // Check if at a junction and a turn hasn't been executed yet
     if (is_at_junction(left_obstacle_value, front_obstacle_value, right_obstacle_value) != 0) {
        // Execute the current turn in the sequence
        if (strcmp(turn_sequence[turn_sequence_index], "180") == 0) {
            turn_180(left_motor, right_motor);
            
            move_forward(left_motor, right_motor);
           
            wb_robot_step(18000);
            printf("Turned 180 degrees\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "right") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4000);
            printf("Turned right\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "left2") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(15500);
            printf("Turned left2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned left\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left3") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(8500);
            printf("Turned left3\n");
        }


        turn_sequence_index++; // Move to the next turn in the sequence
       // turn_executed = 1; // Set the flag to indicate a turn has been executed
         
      }
    
    // Move forward after turning
    move_forward(left_motor, right_motor);
   } 
}

//............red to green final path............

void red_to_green(WbDeviceTag left_motor, WbDeviceTag right_motor, int left_obstacle_value, int front_obstacle_value, int right_obstacle_value) {
    static int turn_sequence_index = 0; // Static variable to track the turn sequence
    
   // static int turn_executed = 0; // Flag to indicate if a turn has been executed
    // Define the turn sequence
   // const char *turn_sequence[] = { "left3", "left3", "left", "right", "left4", "right3","right","right2","left","left","left", "right", "left2", "right", "right", "left2"}; 
    const char *turn_sequence[] = {"180", "right", "right2", "right4", "left2", "right2", "right","left3","left","right2","right3","left","left4","right2","left","left4","right","180", "left4", "right2", "right3", "left5", "right2", "right2","left","left","right","right6","180a", "righta", "left2a", "righta", "lefta", "left3a", "left3a","righta","right","left4a","left","righta","left4a"};
   // Check if at a junction and a turn hasn't been executed yet
    int sequence_length = sizeof(turn_sequence) / sizeof(turn_sequence[0]);
  while (turn_sequence_index < sequence_length) {
   // Check if at a junction and a turn hasn't been executed yet
     if (is_at_junction(left_obstacle_value, front_obstacle_value, right_obstacle_value) != 0) {
        // Execute the current turn in the sequence
        if (strcmp(turn_sequence[turn_sequence_index], "180") == 0) {
            turn_180(left_motor, right_motor);
            
            move_forward(left_motor, right_motor);
           
            wb_robot_step(8000);
            printf("Turned 180 degrees\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "right") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(7100);
            printf("Turned right\n");
        } else if (strcmp(turn_sequence[turn_sequence_index], "left2") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(16000);
            printf("Turned left2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned left\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "right2") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4000);
            printf("Turned right2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left3") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(8000);
            printf("Turned left3\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "right3") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(5000);
            printf("Turned right3\n");   
        }else if (strcmp(turn_sequence[turn_sequence_index], "left4") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(14000);
            printf("Turned left4\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "right4") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(2300);
            printf("Turned right4\n");
         }else if (strcmp(turn_sequence[turn_sequence_index], "left5") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(12000);
            printf("Turned left5\n");
         }else if (strcmp(turn_sequence[turn_sequence_index], "right6") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(11500);
            printf("Turned right6\n");
         }else if (strcmp(turn_sequence[turn_sequence_index], "180a") == 0) {
            turn_180(left_motor, right_motor);
            
            move_forward(left_motor, right_motor);
           
            wb_robot_step(5000);
            printf("Turned 180 degrees\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "righta") == 0) {
            turn_right(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(3500);
            printf("Turned right\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left2a") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
           
            wb_robot_step(16800);
            printf("Turned left2\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "lefta") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(4500);
            printf("Turned left\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left3a") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(9000);
            printf("Turned left3\n");
        }else if (strcmp(turn_sequence[turn_sequence_index], "left4a") == 0) {
            turn_left(left_motor, right_motor);
            move_forward(left_motor, right_motor);
            
            wb_robot_step(25000);
            printf("Turned left3\n");
         }

        turn_sequence_index++; // Move to the next turn in the sequence
       // turn_executed = 1; // Set the flag to indicate a turn has been executed
         
      }
    
    // Move forward after turning
    move_forward(left_motor, right_motor);
   } 
}
// Similarly, modify other navigation functions (yellow_to_red, etc.)
// to use their respective junction direction arrays.
//void brown_to_red(int left_obstacle_value,int front_obstacle_value, int right_obstacle_value){


//}
//void green_to_red(int left_obstacle_value,int front_obstacle_value,int right_obstacle_value){


//}





void run_robot() {
    wb_robot_init();

    int left_obstacle_value;
    int front_obstacle_value;
    int right_obstacle_value;
    // Initialize motors
    WbDeviceTag left_motor = wb_robot_get_device("left wheel motor");
    WbDeviceTag right_motor = wb_robot_get_device("right wheel motor");
    wb_motor_set_position(left_motor, INFINITY);
    wb_motor_set_velocity(left_motor, 0.0);
    wb_motor_set_position(right_motor, INFINITY);
    wb_motor_set_velocity(right_motor, 0.0);

    //int timestep = wb_robot_get_basic_time_step();
    double max_speed = 6.28;

    // Initialize wheel encoders
   // WbDeviceTag left_encoder = wb_robot_get_device("left wheel sensor");
    //WDeviceTag right_encoder = wb_robot_get_device("right wheel sensor");
    //wb_position_sensor_enable(left_encoder, timestep);
    //wb_position_sensor_enable(right_encoder, timestep);

    // Initialize distance sensors
    WbDeviceTag ds_front = wb_robot_get_device("Front sensor");
    WbDeviceTag ds_left = wb_robot_get_device("Left sensor");
    WbDeviceTag ds_left_corner = wb_robot_get_device("Left corner sensor");
    WbDeviceTag ds_right = wb_robot_get_device("Right sensor");
    wb_distance_sensor_enable(ds_left, TIME_STEP);
    wb_distance_sensor_enable(ds_left_corner, TIME_STEP);
    wb_distance_sensor_enable(ds_right, TIME_STEP);
    wb_distance_sensor_enable(ds_front, TIME_STEP);

    // Initialize camera
    WbDeviceTag camera = wb_robot_get_device("camera");
    wb_camera_enable(camera, TIME_STEP);

    // Define color sequence
    const char *color_sequence[] = {"RED", "YELLOW", "PINK", "BROWN", "GREEN"};
    int color_index = 0;

    //double current_x = INITIAL_X;
    //double current_y = INITIAL_Y;
    
    int first_color_detected = 0; 
    int first_color_red_detected = 0; 
    int first_color_yellow_detected = 0; 
    int first_color_pink_detected = 0; 
    int first_color_green_detected = 0; 
    int first_color_brown_detected = 0; 
    

    while (wb_robot_step(TIME_STEP) != -1) {
        // Read sensor values
        printf("Sensor left (value): %f\n",  wb_distance_sensor_get_value(ds_left));
        printf("Sensor left (corner): %f\n",  wb_distance_sensor_get_value(ds_left_corner));
        printf("Sensor front (value): %f\n",  wb_distance_sensor_get_value(ds_front));
        printf("Sensor right (value): %f\n",  wb_distance_sensor_get_value(ds_right));

        //double delta_x, delta_y;
        //get_robot_position(left_encoder, right_encoder, &delta_x, &delta_y);
        //current_x += delta_x;
        //current_y += delta_y;

        //printf("Current Position: (%.3f, %.3f)\n", current_x, current_y);

        // Determine walls and obstacles
        // int left_obstacle_value = wb_distance_sensor_get_value(ds_left);
        // int front_obstacle_value = wb_distance_sensor_get_value(ds_front);
        // int left_corner_obstacle_value = wb_distance_sensor_get_value(ds_left_corner);
        // int right_obstacle_value = wb_distance_sensor_get_value(ds_right);
        
        //if wb_distance_sensor_get_value(ds_left)==1000{
          //left_obstacle_value=1
        //}else if wb_distance_sensor_get_value(ds_left)==600{
          //left_obstacle_value=0
        //}
        //if wb_distance_sensor_get_value(ds_front)==1000{
          //front_obstacle_value=1
        //}else if wb_distance_sensor_get_value(ds_front)==600{
         // front_obstacle_value=0
        //} 
        //if wb_distance_sensor_get_value(ds_right)==1000{
          //right_obstacle_value=1
        //}else if wb_distance_sensor_get_value(ds_right)==600{
          //right_obstacle_value=0
        //} 
          
       // int left_wall = wb_distance_sensor_get_value(ds_left) < 600;
        //int front_wall = wb_distance_sensor_get_value(ds_front) < 600;
        //int left_corner = wb_distance_sensor_get_value(ds_left_corner) < 600;

        double left_sensor_value = wb_distance_sensor_get_value(ds_left);
        double front_sensor_value = wb_distance_sensor_get_value(ds_front);
        double right_sensor_value = wb_distance_sensor_get_value(ds_right);
        double left_corner_sensor_value = wb_distance_sensor_get_value(ds_left_corner);

// Assuming values below THRESHOLD indicate an obstacle
         

        left_obstacle_value = (left_sensor_value < THRESHOLD) ? 0 : 1;
        front_obstacle_value = (front_sensor_value < THRESHOLD) ? 0 : 1;
        right_obstacle_value = (right_sensor_value < THRESHOLD) ? 0 : 1;

       int left_wall = (left_sensor_value < THRESHOLD);  
       int front_wall = (front_sensor_value < THRESHOLD);
       int left_corner = (left_corner_sensor_value < THRESHOLD);
        
        // if (front_wall) {
        // Robot is close to a wall in front, turn!
//            turn_right(left_motor, right_motor); 
        //}// else if (left_wall) {
        // Robot is close to a wall on the left, move forward
           // move_forward(left_motor, right_motor);
        //} else {
        // No immediate walls, continue with other navigation logic
        // ...
        //}
    
        
        // Get the camera image
        const unsigned char *image = wb_camera_get_image(camera);
        int width = wb_camera_get_width(camera);
        int height = wb_camera_get_height(camera);

        printf("Image pointer address: %p\n", (void *)image);
        printf("Image width: %d\n", width);
        printf("Image height: %d\n", height);

        // Detect the color
        const char *current_color = detect_color(image, width, height);
        printf("Detected Color: %s\n", current_color);
        
        // switch (current_color) {
        // case "RED":
            // red_to_green(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // break;
        // case "YELLOW":
            // yellow_to_red(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // red_to_green(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // break;
        // case "PINK":
            // pink_to_red(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // red_to_green(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // break;
        // case "BROWN":
            // brown_to_red(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // red_to_green(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // break;
        // case "GREEN":
            // green_to_red(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // red_to_green(left_obstacle_value,front_obstacle_value,right_obstacle_value);
            // break;
        // default:
     
            // break;
        // }
        
        //---------------------------------------------new code starts
        if (strcmp(current_color, color_sequence[color_index]) == 0) {
            color_index++;
            
            wb_motor_set_velocity(left_motor, 0.0);
            wb_motor_set_velocity(right_motor, 0.0);
            wb_robot_step(1000); // Pause for a moment
            if (color_index == 5) break; // Stop if the last color (GREEN) is reached
       }

        double left_speed = max_speed;
        double right_speed = max_speed;
        wb_motor_set_velocity(left_motor, left_speed);
        wb_motor_set_velocity(right_motor, right_speed); 
        //------------------------------------------------------------
        
        if (strcmp(current_color, "RED") == 0) {
               first_color_red_detected = 1; 
               first_color_yellow_detected = 0; 
               first_color_pink_detected = 0; 
               first_color_green_detected = 0; 
               first_color_brown_detected = 0; 
          
        }
         if (strcmp(current_color, "YELLOW") == 0) {
               first_color_red_detected = 0; 
               first_color_yellow_detected = 1; 
               first_color_pink_detected = 0; 
               first_color_green_detected = 0; 
               first_color_brown_detected = 0; 
          
        }
         if (strcmp(current_color, "PINK") == 0) {
               first_color_red_detected = 0; 
               first_color_yellow_detected = 0; 
               first_color_pink_detected = 1; 
               first_color_green_detected = 0; 
               first_color_brown_detected = 0; 
          
        }
         if (strcmp(current_color, "GREEN") == 0) {
               first_color_red_detected = 0; 
               first_color_yellow_detected = 0; 
               first_color_pink_detected = 0; 
               first_color_green_detected = 1; 
               first_color_brown_detected = 0; 
          
        }
         if (strcmp(current_color, "BROWN") == 0) {
               first_color_red_detected = 0; 
               first_color_yellow_detected = 0; 
               first_color_pink_detected = 0; 
               first_color_green_detected = 0; 
               first_color_brown_detected = 1; 
          
        }
        
        //-----------------------------------------------------------
        printf("Detected Color: %s\n", current_color);
        
         // Check for the first color detection (and only the first)
        if (strcmp(current_color, "NONE") != 0 && !first_color_detected) {
            first_color_detected = 1; 
           // wb_camera_disable(camera);
           // wb_camera_disable(camera); 
            //printf("Camera disabled.\n");
            // Set the flag permanently to 1
            printf("First color detected, switching to color navigation\n");
        }
        printf("First Detected Color: %d\n", first_color_detected);
        printf("First Detected Color pink: %d\n", first_color_pink_detected);
        if (!first_color_detected) {
            // Left wall following (only if first_color_detected is 0)
             left_wall_follow( max_speed,  front_wall,  left_wall,  left_corner,  left_motor,  right_motor,  left_speed,  right_speed);
        } else {
            // Color navigation (if first_color_detected is 1)
            // ... (Your color navigation logic using pink_to_red, etc.) ...
           // int i;
           
           if (first_color_red_detected==1) {
             red_to_green( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
             // for (i = 0; i < sizeof(junction_directions_pink_to_red) / sizeof(junction_directions_pink_to_red[0]); i++) 
             // {
              // printf("Junction Turn: %s\n", junction_directions_pink_to_red[i].direction); 
             // }
              

          } 
          if (first_color_yellow_detected==1) {
             yellow_to_red( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
             red_to_green( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
            
          } 
           if (first_color_pink_detected==1) {
             pink_to_red( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
           //  red_to_green( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
            
          } 
           if (first_color_green_detected==1) {
             printf("going to start green to redn\n");
             green_to_red( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
             printf("Finished green to red, going to start red to green\n");
             red_to_green( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
             printf("Finished red to greenion\n");
          } 
           if (first_color_brown_detected==1) {
             brown_to_red( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
            // red_to_green( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
            
          } 
           
          
        }
        
        //---------------------------------------------new code ends
        
        //-----------------------------------------------old code starts
          //int i;
       //if (strcmp(current_color, "RED") == 0) {
         // red_to_green(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value); 
       //} else if (strcmp(current_color, "YELLOW") == 0) {
          //yellow_to_red(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
          // red_to_green(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
          // if (strcmp(current_color, "PINK") == 0) {
             // pink_to_red( left_motor,  right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
             // for (i = 0; i < sizeof(junction_directions_pink_to_red) / sizeof(junction_directions_pink_to_red[0]); i++) 
             // {
              // printf("Junction Turn: %s\n", junction_directions_pink_to_red[i].direction); 
             // }
           // }  
             //printf("Junction Turn: %s\n", junction_directions_pink_to_red[i].direction);

             //red_to_green(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
          //} else if (strcmp(current_color, "BROWN") == 0) {
              //brown_to_red(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
              //red_to_green(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
          //} else if (strcmp(current_color, "GREEN") == 0) {
               // green_to_red(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
                //red_to_green(left_motor, right_motor, left_obstacle_value, front_obstacle_value, right_obstacle_value);
           //} else {
    //Do nothing for default case
          // }
            //printf("Detected Color: %s\n", current_color);

        //Check if the detected color matches the current target in the sequence
        // if (strcmp(current_color, color_sequence[color_index]) == 0) {
            // color_index++;
            // wb_motor_set_velocity(left_motor, 0.0);
            // wb_motor_set_velocity(right_motor, 0.0);
            // wb_robot_step(1000); // Pause for a moment
            // if (color_index == 5) break; // Stop if the last color (GREEN) is reached
        // }

        // double left_speed = max_speed;
        // double right_speed = max_speed; left_wall_follow(double max_speed, int front_wall, int left_wall, int left_corner, int left_motor, int right_motor, int left_speed, int right_speed);
        // wb_motor_set_velocity(left_motor, left_speed);
        // wb_motor_set_velocity(right_motor, right_speed); 
        // void


       //left wall algorithm cut
       //-----------------------------------------------------------------------old code ends
    }

    wb_robot_cleanup();
}

int main() {
    run_robot();
    return 0;
}


