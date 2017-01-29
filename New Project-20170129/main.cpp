#include <iostream>
#include <queue>
#include <fstream>
#include "math.h"


using namespace std;

int main()
{
   cout << "motion profile tester" << endl; 
   
   queue<double> vel;
   
   //function arguments
   float dist_to_travel = 200;  //in Inches
   float entered_velocity = 70; //Inches per second
   
   //needed constants
   double dt = 0.02;             //time step in seconds
   int drivetrain_max_accel_rate = 70; //in Inches per second^2
   
   //begin.  check if a triangle profile is needed or a trapezoid.
   // Rearranging v^2 = 2as for s
   float accel_dist = 0.5*entered_velocity*entered_velocity/drivetrain_max_accel_rate;
   cout <<"accel_dist: "<< accel_dist << endl;
   
   
   float max_speed = sqrt(2*drivetrain_max_accel_rate*accel_dist);
   
   if(accel_dist*2 > dist_to_travel){ 
       //we will never reach the requested speed, so a triangle profile is generated
       cout <<"triangle profile" << endl;
       int accel_segments = ceil(entered_velocity / drivetrain_max_accel_rate / dt);
    
        
        cout << "Points:" << accel_segments*2 << endl;
        
       //generate accel curve
       for(int i = 0;i <= accel_segments;i++) {
         double t = dt*i;
         vel.push(drivetrain_max_accel_rate*t);
       }
       double initial_v = vel.back();
       cout <<"initV "<<initial_v<<endl;
       
       //generate decel curve
       for(int i=0 ;i < accel_segments;i++) {
         double t = dt*i;
         // v = u + at
         double velocity = initial_v+(-drivetrain_max_accel_rate*t);
        if(velocity > 0)
         vel.push(velocity);
       }
       //push last point
       vel.push(0);
   }
   else {  
       //need to create Trapezoid
       cout << "trapezoid profile" << endl;
       
       double hold_distance = dist_to_travel - (accel_dist * 2);
       double hold_time = hold_distance/entered_velocity;
        
        int accel_segments = ceil(entered_velocity / drivetrain_max_accel_rate / dt);
        int hold_segments = hold_time / dt;
        
        cout <<"accel segments " << accel_segments << endl;
        cout <<"hold_distance " << hold_distance << endl;
        cout <<"hold_segments " << hold_segments << endl;
        
        
        //generate the accel
        for(int i = 0;i <= accel_segments;i++) {
         float t = dt*i;
         vel.push(drivetrain_max_accel_rate*t);
       }
       double initial_v = vel.back();
       cout <<"speed reached "<<initial_v<<endl;
        
        //generate the hold
        for(int y = 0;y < hold_segments;y++) {
            vel.push(entered_velocity);
        }
        
        //generate the decel
        for(int i=0 ;i < accel_segments;i++) {
         double t = dt*i;
         // v = u + at
         vel.push(entered_velocity+(-drivetrain_max_accel_rate*t));
        }   
    
       //push last point
       vel.push(0);
    }
   //print the profile
   
   ofstream file;
   file.open("output.csv");
   int x=0;
   cout<<"size:"<<vel.size() << endl;
   while(!vel.empty()) {
       //cout <<x<<","<< vel.front() << "\n"<<endl;
       file <<x*dt<<","<< vel.front() << "\n";
       vel.pop();
       x++;
   }
   file.close();
   return 0;
}


